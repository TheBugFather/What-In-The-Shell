import sys
from pathlib import Path


def print_err(err_message):
    print(f'\n* [ERROR] {err_message} *\n', file=sys.stderr)


def main():
    # If the file path arg was not passed in #
    if len(sys.argv) < 2:
        # Print error and exit #
        print_err("User failed to pass in file path arg to parse")
        sys.exit(-1)

    # Parse the file path arg as file path object #
    file_path = Path(sys.argv[1])

    # If the file path arg does not exist on disk or is not cpp code #
    if not file_path.exists() or not str(file_path).endswith('.cpp'):
        # Print error and exit #
        print_err("Passed in file path does not exist on disk")
        sys.exit(-2)

    # Format the output file based on validated input file #
    out_path = Path(f'{file_path}_parse.txt')

    try:
        # Open the source code file in read mode #
        with file_path.open('r', encoding='utf-8') as in_file, \
             out_path.open('w', encoding='utf-8') as out_file:
            # Iterate through the open source file line by line #
            for line in in_file:
                # Copy original line to buffer for parsing #
                buffer = line

                # If there is a tab in the buffer #
                if '\t' in buffer:
                    # Replace any tabs in the line with spaces #
                    buffer = buffer.replace('\t', '    ')

                # Preserve the newlines from original line #
                buffer = repr(f'\"{buffer}\"')
                # Remove extra single quotes from prior line #
                buffer = buffer.replace('\'', '')

                # Write the resulting output as string #
                out_file.write(buffer + '\n')

    # If error occurs during file operation #
    except OSError as file_err:
        # Print error and exit #
        print_err(f'File error occured during parsing: {file_err}')
        sys.exit(-3)


if __name__ == '__main__':
    main()
