#!/usr/bin/env python3
import sys
import os
import glob
from google import genai

def gather_codebase_context(root_dir, max_files=40):
    """Scans workspace files to generate prompt context."""
    context = []
    allowed_exts = ('.cpp', '.h', '.hpp', '.qml', '.ui', 'CMakeLists.txt', '.pro')
    
    for root, _, files in os.walk(root_dir):
        if any(ignored in root for ignored in ['build', '.git', '.devcontainer', '__pycache__']):
            continue
        for file in files:
            if file.endswith(allowed_exts):
                full_path = os.path.join(root, file)
                try:
                    with open(full_path, 'r', encoding='utf-8') as f:
                        content = f.read()[:3000] # Cap per file snippet
                        context.append(f"// FILE: {file}\n{content}")
                except Exception:
                    pass
                if len(context) >= max_files:
                    break
    return "\n\n".join(context)

def main():
    if len(sys.argv) < 2:
        print("// No prompt provided.")
        sys.exit(1)
        
    prompt = " ".join(sys.argv[1:])
    workspace_dir = os.getcwd()
    
    api_key = os.getenv("GEMINI_API_KEY")
    if not api_key:
        print("// Error: GEMINI_API_KEY environment variable is missing.")
        sys.exit(1)

    code_context = gather_codebase_context(workspace_dir)
    
    system_instruction = f"""You are an expert Qt/C++ assistant integrated into Qt Creator IDE.
Generate clean C++/QML code snippets or clear debugging insights based on the codebase context below:

=== CODEBASE CONTEXT ===
{code_context}

=== USER PROMPT ===
{prompt}
"""

    client = genai.Client(api_key=api_key)
    response = client.models.generate_content(
        model="gemini-2.5-flash",
        contents=system_instruction
    )
    
    print(response.text)

if __name__ == "__main__":
    main()