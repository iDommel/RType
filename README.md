# RType
Repository for the R-Type project

# Compilation:
    sh install.sh [Optional build type arg=(Release/Debug/ect)]

# Recompilation (Debug or anything else)
    sh reBuild.sh

# Alternative way:
    chmod +x install.sh reBuild.sh
    "now use [./] instead of [sh ] for above commands"

# Usage
The Server has to be launched BEFORE the clients for the connection to work.


    ./r-type_server SERVER_IP_ADDRESS SERVER_PORT
    ./r-type_client SERVER_IP_ADDRESS SERVER_PORT

Example to launch game on Localhost:8080

    ./r-type_server "127.0.0.1" 8080
    ./r-type_client "127.0.0.1" 8080

# Language:
Use english EVERYWHERE !

# Pull requests:
A pull request needs to be made before merging on dev and main.
To create a pull request:
    - Go on https://github.com/iDommel/RType
    - Click on the "Pull requests" tab -> "New pull request"
    - Select dev as "base:" and the branch you want to merge as "compare:"
    - Click on "Create pull request"
    - Enter a title and a description for your pull request
    - Select 2 reviewers fitted to review your code (Reviewers tab is at the right of the description)
    - Click again on "Create pull request" => Your pull request is created
    - If one or more reviewers asked for changes, just push your changes and your pull request will update accordingly
    - Once your pull request is validated by all the reviewers you can click on "Merge pull request" => Your pull request is merged

# Commit norm:
Conjugated verb in preterit form and in upper case: action
Examples -> ADDED: run() function in Core.cpp
         -> MODIFIED: Entity class in Entity.hpp

# Commits:
To undo last commit and to restore file content to the prevous commit:
    git reset --hard HEAD^
Same thing but keeping your changes:
    git reset HEAD^