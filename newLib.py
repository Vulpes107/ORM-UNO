import os

def create_class_files(class_name, base_path):
    # Create folder
    folder_name = os.path.join(base_path, class_name)
    os.makedirs(folder_name, exist_ok=True)

    # File paths
    hpp_path = os.path.join(folder_name, f"{class_name}.hpp")
    cpp_path = os.path.join(folder_name, f"{class_name}.cpp")

    # Class template for hpp
    hpp_content = f"""#ifndef {class_name.upper()}_HPP
#define {class_name.upper()}_HPP

class {class_name} {{
private:
    // Add private fields and methods here

public:
    {class_name}();  // Constructor
    ~{class_name}(); // Destructor
}};

#endif // {class_name.upper()}_HPP
"""

    # Class template for cpp
    cpp_content = f"""#include "{class_name}.hpp"

{class_name}::{class_name}() {{

}}

{class_name}::~{class_name}() {{

}}
"""

    # Write hpp file
    with open(hpp_path, "w") as hpp_file:
        hpp_file.write(hpp_content)

    # Write cpp file
    with open(cpp_path, "w") as cpp_file:
        cpp_file.write(cpp_content)

    print(f"Class {class_name} created with {hpp_path} and {cpp_path}.")

if __name__ == "__main__":
    import tkinter as tk
    from tkinter import filedialog

    # Prompt for class name
    class_name = input("Enter the class name: ").strip()

    # Validate class name
    if not class_name.isidentifier():
        print("Invalid class name. Please use a valid identifier.")
    else:
        # Open file dialog to select path
        root = tk.Tk()
        root.withdraw()  # Hide the root window
        print("Select the folder where you want to create the class files.")
        base_path = filedialog.askdirectory()

        if base_path:  # Proceed only if a folder is selected
            create_class_files(class_name, base_path)
        else:
            print("No folder selected. Exiting.")
