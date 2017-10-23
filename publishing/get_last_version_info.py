import argparse
import json

parser = argparse.ArgumentParser()
group = parser.add_mutually_exclusive_group(required=False)
group.add_argument("--version", help="Prints only newest version from changelog", dest="version", action="store_true")
group.add_argument("--comment", help="Prints only comment of the newest version from changelog", dest="comment", action="store_true")
parser.add_argument("changelog_file", help="Path to the changelog file")
args = parser.parse_args()

with open(args.changelog_file, "r") as json_file:
    version_info = json.load(json_file)["versions"]
    newest_idx = 0

    def get_version():
        return version_info[newest_idx]["version"]

    def get_comment():
        return version_info[newest_idx]["comment"]

    if(args.version):
        print get_version()
    elif(args.comment):
        print get_comment()
    else:
        print "{version}: {comment}".format(version=get_version(), comment=get_comment())
