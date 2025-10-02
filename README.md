
# UniHub CLI

>A C++17 command-line application for NIT Trichy students to share and access academic resources, with file-based storage and a menu-driven interface.

---

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Data Layout](#data-layout)
- [Supported Branches](#supported-branches)
- [Installation & Build](#installation--build)
- [Usage](#usage)
- [Commands & Menu Flow](#commands--menu-flow)
- [Security Notes](#security-notes)
- [Contributing](#contributing)
- [License](#license)

---

## Overview
UniHub CLI is a mock academic resource sharing platform for NIT Trichy, implemented in modern C++ (C++17). It allows students to register, log in, manage their profiles, and share/download subject-specific resources, all via a simple terminal interface. Data is stored locally in a structured file system layout.

## Features
- **User Registration & Login**: College email-based authentication, password stored as salted hash (local only).
- **Profile Management**: View and edit profile (name, email, year, semester, branch, section).
- **Subject & Resource Browsing**: Auto-generated subjects per branch/year/semester/section, with teacher assignment.
- **Resource Management**: Browse, upload, and download files for each subject and resource type.
- **File Storage**: Organized by academic hierarchy (year, semester, branch, section, subject, resource type).
- **No External Dependencies**: Pure C++17, uses only standard library and `std::filesystem`.

## Architecture
- **Source Code**: Located in `Code/src/` and headers in `Code/include/`.
- **Main Components**:
	- `auth`: User registration, login, profile management.
	- `menu`: Main menu and user interaction logic.
	- `subjects`: Subject and curriculum management.
	- `resources`: Resource file management (list/upload/download).
	- `storage`: File and directory utilities.
- **Entry Point**: `main.cpp` calls `uni::runApp()` to launch the menu system.

## Data Layout
- **Users**: `Code/data/users/*.profile` (CSV profile) and `*.cred` (salt + password hash).
- **Resources**: `Code/data/resources/{year}/{semester}/{branch}/{section}/{subject}/{type}/`
	- Example: `Code/data/resources/2/3/CSE/B/Data Structures/Notes/Data Structures.pdf`

## Supported Branches
- CSE, ECE, EEE, ICE, ME, CHE, PROD, CIVIL, MME, ARCH
	- Architecture: 5 years (10 semesters)
	- Other branches: 1-8 semesters

## Installation & Build
1. **Clone the repository**
2. **Build using Make**:
	 ```sh
	 cd Code
	 make
	 ```
3. **Run the application**:
	 ```sh
	 make run
	 ```
	 Or execute the binary directly:
	 ```sh
	 ./bin/unihub
	 ```

## Usage
On running, you'll see a menu:

```
===== Welcome to UniHub CLI =====
1) Login
2) Register
0) Exit
Choose:
```

### Registration
- Enter your details (name, college email, password, year, semester, branch, section)
- Branch input accepts both full name and code (e.g., "CSE" or "Computer Science and Engineering")

### Login
- Enter your college email and password

### Main Menu
- View/Edit Profile
- Browse Subjects & Resources
	- Select subject → select resource type (Notes, Assignments, PPTs, etc.) → list/upload/download files

## Commands & Menu Flow
- **Profile**: View and edit your profile
- **Subjects & Resources**: Browse subjects, view teachers, access resource folders
- **Upload**: Add files to resource folders
- **Download**: Retrieve files from resource folders

## Security Notes
- Passwords are hashed with a random salt using `std::hash`. This is **not cryptographically secure** and is only suitable for offline/mock use.
- All data is stored locally; there is no network or cloud integration.

## Contributing
Pull requests and suggestions are welcome! Please open an issue for major changes or feature requests.

## License
This project is for educational/mock purposes and is not intended for production use.
