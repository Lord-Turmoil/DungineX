import datetime
import os

WIDTH = 79

# Get today's date in Month Day, Year format
TODAY = datetime.datetime.now().strftime("%B %d, %Y")


def update_impl(prefix: str, file_path: str):
    with open(file_path, "r") as f:
        lines = f.readlines()

    new_lines = []
    for line in lines:
        if line.startswith(prefix):
            new_line = prefix
            new_line += TODAY
            while len(new_line) < WIDTH - 1:
                new_line += " "
            new_line += "*"
        else:
            new_line = line.rstrip()
        new_lines.append(new_line)

    # write back as Windows CRLF
    with open(file_path, "w") as f:
        f.write("\n".join(new_lines))


def update_start_date(file_path: str):
    PREFIX = " *                     Start Date : "
    update_impl(PREFIX, file_path)


def update_update_date(file_path: str):
    PREFIX = " *                    Last Update : "
    update_impl(PREFIX, file_path)


def update_headers():
    # run git status --porcelain to get the list of modified files
    with os.popen("git status --porcelain") as f:
        for line in f:
            status, file_path = line[:2].strip(), line[3:].strip()
            if not file_path.startswith("DungineX/"):
                continue
            print(f"Updating header for {file_path} (status: {status})")
            if status == "A" or status == "??":
                update_start_date(file_path)
            update_update_date(file_path)


if __name__ == "__main__":
    update_headers()
