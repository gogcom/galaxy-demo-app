import os
import argparse
import json
import sys
import logging
from platform import system
from subprocess import check_call
from tempfile import mkdtemp
from shutil import copy2

game_binaries = "output"

parser = argparse.ArgumentParser()
parser.add_argument("--login", required=True, help="User name used for authentication on GOG.com servers")
parser.add_argument("--password", required=True, help="Password used for authentication on GOG.com server")
parser.add_argument("--product_id", required=True, help="Game base product id")
parser.add_argument("--branch", required=True, help="Game branch")
parser.add_argument("--branch_password", required=True, help="Game branch password")
parser.add_argument("--galaxy_sdk_root", required=True, help="Path to the Galaxy SDK")
parser.add_argument("--pipeline_builder", required=True, help="Path to the pipeline buidler binary")
parser.add_argument("--version", help="Set game release version explicitly, instead of incrementing it")
args = parser.parse_args()

galaxy_sdk_lib = os.path.join(args.galaxy_sdk_root, "Libraries")
if not os.path.isdir(galaxy_sdk_lib):
    logging.error("Cannot find Galaxy SDK libraries: %s", galaxy_sdk_lib)
    sys.exit(1)

if not os.path.isfile(args.pipeline_builder):
    logging.error("Cannot find pipeline builder: %s", args.pipeline_builder)
    sys.exit(1)

workdir = mkdtemp(prefix="game_publisher_")
logging.info("Working in %s", workdir)

repo_dir = os.path.join(workdir, "repo")
os.mkdir(repo_dir)
logging.info("Download repository: product_id=%s, branch=%s, out_dir=%s", args.product_id, args.branch, repo_dir)

check_call([
    args.pipeline_builder,
    "download-repository",
    args.product_id,
    repo_dir,
    "--username={}".format(args.login),
    "--password={}".format(args.password),
    "--silent",
    "--branch={}".format(args.branch),
    "--branch_password={}".format(args.branch_password)
])

extract_dir = os.path.join(workdir, "extract")
os.mkdir(extract_dir)
logging.info("Extracting game files: in_dir=%s, out_dir=%s", repo_dir, extract_dir)

check_call([
    args.pipeline_builder,
    "extract-repository",
    repo_dir,
    extract_dir,
    "--silent"
])

logging.info("Updating binaries: game_binaries={game_binaries}, galaxy_sdk_lib={galaxy_sdk_lib}", game_binaries, galaxy_sdk_lib)
if system() == "Darwin":
    copy2(
        os.path.join(game_binaries, "gogtron.app", "Contents", "MacOS", "gogtron"),
        os.path.join(extract_dir, args.product_id, "languages_p_en", "__game", "Contents", "MacOS")
    )
    copy2(
        os.path.join(galaxy_sdk_lib, "libGalaxy.dylib"),
        os.path.join(extract_dir, args.product_id, "languages_p_en", "__game", "Contents", "Frameworks")
    )
elif system() == "Windows":
    for src in (os.path.join(galaxy_sdk_lib, "Galaxy.dll"), os.path.join(game_binaries, "gogtron.exe")):
        copy2(src, os.path.join(extract_dir, args.product_id, "languages_p_en", "__game"))
else:
    logging.error("Unsupported platform: %s", system())
    sys.exit(2)

project_json = os.path.join(extract_dir, "project.json")
with open(project_json, "r+") as json_file:
    data = json.load(json_file)

    data["project"]["version"] = args.version
    logging.info("Setting game version: %s", data["project"]["version"])

    json_file.seek(0)
    json.dump(data, json_file, indent=4, sort_keys=True)
    json_file.truncate()

build_dir = os.path.join(workdir, "build")
os.mkdir(build_dir)
logging.info("Compiling and releasing game: build_dir=%s, release_branch=%s", build_dir, args.branch)

check_call([
    args.pipeline_builder,
    "build-game",
    project_json,
    "--output={}".format(build_dir),
    "--silent",
    "--username={}".format(args.login),
    "--password={}".format(args.password),
    "--branch={}".format(args.branch),
    "--branch_password={}".format(args.branch_password)
])
