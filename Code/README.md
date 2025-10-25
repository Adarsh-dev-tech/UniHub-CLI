# UniHub CLI - Hybrid Data Structure Implementation

A C++17 command-line application for NIT Trichy students to share and access academic resources, implementing a comprehensive hybrid data structure system with file-based storage.

## Features

### 🏗️ **Hybrid Data Structure Architecture**
- **Hash Tables**: O(1) user lookup by email
- **AVL Trees**: Sorted user browsing and balanced access
- **LRU Cache**: Recent user tracking
- **DAG (Directed Acyclic Graph)**: Subject prerequisite management
- **B-Trees**: Efficient resource metadata indexing
- **Tries**: Autocomplete for resource names
- **Priority Queues**: Popular resource ranking
- **Graphs**: User connections and resource relationships
- **Stacks**: Navigation history with breadcrumbs

### 🎓 **Academic Features**
- Register/Login using college email + password (salted hash)
- Editable user profiles (name, email, year, semester, branch, section)
- Subject generation for 10 branches with prerequisite tracking
- Custom curriculum support (CSE Y2S3B implemented)
- Browse subjects → resource types → list/upload/download files

### 📚 **Resource Management**
- 8 resource types: Notes, Assignments, PPTs, EndSemPapers, CTs, MidSemPapers, YouTubeLinks, ReferenceBooks
- Global resource sharing (uploads available to all students)
- Download tracking and popularity ranking
- Search functionality with autocomplete
- File storage: `data/resources/{year}/{semester}/{branch}/{section}/{subject}/{type}/`

### 🏛️ **University Structure**
- **10 Branches**: CSE, ECE, EEE, ICE, ME, CHE, PROD, CIVIL, MME, ARCH
- **Years**: 1-4 (Engineering), 1-5 (Architecture)
- **Semesters**: 1-8 (Engineering), 1-10 (Architecture)
- **Sections**: A and B (different teachers per section)

## Build and Run

```bash
# Build
make

# Run
make run

# Clean build
make clean && make
```

Binary: `bin/unihub`

## Custom Curriculum Example

CSE Year 2, Semester 3, Section B:
- Computer Organization (Prof. Mala)
- Principles of Programming Languages (Prof. Bala)
- Data Structures (Prof. Oswald)
- Probability and Operations Research (Prof. Shivaranjini)
- Combinatorics and Graph Theory (Prof. Pavan)
- Digital Systems Design (Prof. Shameedha)

## Project Structure

```
├── src/
│   ├── main.cpp              # Entry point with enhanced menu
│   ├── auth.cpp              # User authentication & profiles
│   ├── storage.cpp           # File/directory utilities
│   ├── subjects.cpp          # Subject generation & curriculum
│   ├── resources.cpp         # Resource management
│   └── resource_index.cpp    # B-Tree implementation
├── include/
│   ├── enhanced_menu.h       # Main application interface
│   ├── unihub_core.h         # Central data structure hub
│   ├── user_manager.h        # Hash + AVL + LRU user system
│   ├── academic_manager.h    # Tree + DAG academic system
│   ├── resource_index.h      # B-Tree + Trie + Queue system
│   ├── data_structures.h     # Core DS implementations
│   ├── auth.h                # Authentication interfaces
│   ├── storage.h             # File utilities
│   ├── subjects.h            # Subject management
│   └── resources.h           # Resource interfaces
├── data/
│   ├── users/                # User profiles and credentials
│   └── resources/            # Hierarchical resource storage
└── .vscode/
    └── tasks.json            # Build and run tasks
```

## Data Structure Usage Examples

1. **Login**: Hash table lookup → AVL tree for "forgot email" → LRU cache update
2. **Subject Prerequisites**: DAG traversal to show dependency chain
3. **Resource Search**: Trie autocomplete → B-Tree search → Priority queue ranking
4. **Navigation**: Stack-based history with breadcrumb display
5. **Popular Resources**: Priority queue maintains most downloaded content

## Security Note

Password hashing uses `std::hash` with random salts. This is suitable for educational/offline use but not cryptographically secure for production.

## License

MIT