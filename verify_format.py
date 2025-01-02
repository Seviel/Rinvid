import os
import subprocess

def is_clang_formatted(file_path):
    result = subprocess.run(['clang-format-12', '-output-replacements-xml', file_path], capture_output=True, text=True)
    return '<replacement ' not in result.stdout

def check_repo_for_clang_format(repo_path):
    for root, dirs, files in os.walk(repo_path):
        if 'extern' in root:
            continue
        for file in files:
            if file.endswith(('.cpp', '.h')):            
                file_path = os.path.join(root, file)                
                if not is_clang_formatted(file_path):
                    print(f"File {file_path} is not clang-formatted.")
                    return False
    print("All files are clang-formatted.")
    return True
    
if __name__ == "__main__":
    repo_path = './'
    
    if check_repo_for_clang_format(repo_path) == True:
        exit(0)
    
    exit(1)
