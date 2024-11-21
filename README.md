# GitLite

GitLite is a simplified version of a version control system, designed to help users manage their projects with basic commands similar to those found in Git. This project is implemented in C++ and provides a console-based interface for interacting with repositories.

## Features

- Initialize a new repository
- Create and switch branches
- Commit changes with messages
- Merge branches
- View commit history
- Save and load repository state
- Visualize the repository tree (Bonus feature)

## Commands

- `init`: Initialize a new repository.
- `branch <branch_name>`: Create a new branch.
- `checkout <branch_name>`: Switch to the specified branch.
- `commit <message>`: Commit changes with the provided message.
- `branches`: List all branches.
- `delete-branch <branch_name>`: Delete the specified branch.
- `merge <source_branch> <target_branch>`: Merge the source branch into the target branch.
- `visualize-tree <branch_name>`: Visualize the repository tree for the specified branch.
- `log`: View the commit history of the current branch.
- `current-branch`: Display the current branch.
- `save`: Save the repository state to a file.
- `load <filename>`: Load the repository state from a file.
- `exit`: Exit the GitLite application.

## Usage

1. Clone the repository and navigate to the project directory.
2. Compile the project using a C++14 compatible compiler.
3. Run the executable to start the GitLite console.
4. Enter commands as described in the Commands section to interact with the repository.

## Example
```bash
>init test.csv
Which tree would you like to use?
1: AVL
2: B-Tree
3: Red-Black Tree
1
Selected AVL tree.
>branch feature-1
Branch 'feature-1' created successfully.
>checkout feature-1
Switched to branch 'feature-1'.
>commit "Initial commit"
Changes committed with message: "Initial commit".
>log
Commit History for 'feature-1':
Commit #1: "Initial commit".
>save
Repository saved successfully to 'repo_data.txt'.
>exit
Exiting GitLite...
