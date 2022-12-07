
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct Command {
    std::string cmd;
    std::vector<std::string> results;
};

struct File {
    std::string name;
    int size;
};

class Directory {
    public:
        std::string name;
        std::vector<File> files;
        std::vector<Directory*> subdirectories;
        Directory* parent;

        Directory(std::string name, Directory* parent=nullptr) : name(name), parent(parent) {}

        ~Directory() {
            for (Directory* subdir : subdirectories)
                delete subdir;
        }

        Directory* find(std::string dir_name) const {
            for (Directory* subdir : subdirectories) {
                if (subdir->name == dir_name)
                    return subdir;
            }
            return nullptr;
        }

        int get_size() {
            if (_size == -1) {
                int size = 0;
                for (File file : files)
                    size += file.size;
                for (Directory* subdir : subdirectories)
                    size += subdir->get_size();
                _size = size;
            }
            return _size;
        }

        int get_filtered_size() {
            int size = 0;
            if (get_size() < 100000)
                size += get_size();
            for (Directory* subdir : subdirectories)
                size += subdir->get_filtered_size();
            return size;
        }

        Directory* find_smallest(int size) {
            Directory* smallest = (get_size() > size ? this : nullptr);
            for (Directory* subdir : subdirectories) {
                Directory* smallest_subdir = subdir->find_smallest(size);
                if (!smallest_subdir)
                    continue;

                int subdir_size = smallest_subdir->get_size();
                if (subdir_size > size && (!smallest || subdir_size < smallest->get_size()))
                    smallest = smallest_subdir;
            }
            return smallest;
        }

    private:
        int _size = -1;
};


int main(int argc, char* argv[])
{
    bool first_half = true;
	int ans = 0;

    std::ifstream myfile("input");
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-t" || arg == "--test")
            myfile = std::ifstream("test_input");

        else if (arg == "-s" || arg == "--second")
            first_half = false;
    }

    std::vector<Command> commands;
	if (myfile.is_open()) {
        std::string input;

        std::string cmd;
        std::vector<std::string> results;
        while (getline(myfile, input)) {
            if (input[0] == '$') {
                if (!cmd.empty())
                    commands.push_back(Command{cmd, results});
                cmd = input.substr(2);
                results.clear();
                continue;
            }

            results.push_back(input);
        }
        if (!cmd.empty())
            commands.push_back(Command{cmd, results});

		myfile.close();
	} else {
        std::cout << "Unable to open file" << std::endl;
        return 1;
    }

    // Build file tree
    Directory* root = new Directory("");
    Directory* curr_dir = root;
    for (Command cmd : commands) {
        // cd command
        if (cmd.cmd.substr(0, 2) == "cd") {
            if (cmd.cmd == "cd /")
                continue;

            if (cmd.cmd == "cd ..") {
                curr_dir = curr_dir->parent;
            } else {
                std::string name = cmd.cmd.substr(3);
                Directory* subdir = curr_dir->find(name);
                if (!subdir) {
                    subdir = new Directory(name, curr_dir);
                    curr_dir->subdirectories.push_back(subdir);
                }

                curr_dir = subdir;
            }
            continue;
        }

        // ls command
        for (std::string res : cmd.results) {
            if (res.substr(0, 3) == "dir") {
                std::string name = res.substr(4);
                if (!curr_dir->find(name))
                    curr_dir->subdirectories.push_back(new Directory(name, curr_dir));

            } else {
                size_t index = res.find(' ');
                curr_dir->files.push_back(File{res.substr(index+1), std::stoi(res.substr(0, index))});
            }
        }
    }

    if (first_half) {
        ans = root->get_filtered_size();
    } else {
        int unused_space = 70000000 - root->get_size();
        int to_delete = 30000000 - unused_space;
        ans = root->find_smallest(to_delete)->get_size();
    }

    delete root;

    std::cout << "Answer : " << ans << std::endl;
	return 0;
}
