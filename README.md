# UniHub CLI (Mock)

A C++17 command-line app for NIT Trichy students to share and access academic resources with file-based storage.

Features implemented:
- Register/Login using college email + password (simple salted hash stored locally)
- Profile view/edit (first, last, email, year, semester, branch, section)
- Auto-generated subjects based on year/semester/branch/section; different teachers per section
- Browse subjects -> resource types -> list/upload/download files
- File storage: `data/resources/{year}/{semester}/{branch}/{section}/{subject}/{type}/`

Notes:
- Password hashing uses std::hash with a salt. This is NOT cryptographically secure; acceptable for offline mock.
- No external dependencies; uses C++17 and std::filesystem.

## Build and Run

Using Make:

```sh
make
make run
```

Binary: `bin/unihub`

## Branch Codes

- CSE, ECE, EEE, ICE, ME, CHE, PROD, CIVIL, MME, ARCH

Architecture has 5 years (10 semesters). Other branches typically use 1-8 semesters.

## Data Layout

- Users: `data/users/*.profile` and `*.cred`
- Resources: `data/resources/{year}/{semester}/{branch}/{section}/{subject}/{type}/`

