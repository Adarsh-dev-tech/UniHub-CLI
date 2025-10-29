
# UniHub CLI - Advanced Hybrid Data Structure Implementation

> A sophisticated C++17 command-line application for NIT Trichy students implementing comprehensive hybrid data structures for academic resource sharing and management.

---

## 📋 Table of Contents
- [🎯 Overview](#-overview)
- [🏗️ Advanced Architecture](#️-advanced-architecture)
- [🚀 Key Features](#-key-features)
- [📊 Data Structures Implementation](#-data-structures-implementation)
- [🎓 Academic System](#-academic-system)
- [📁 Project Structure](#-project-structure)
- [💾 Data Storage Layout](#-data-storage-layout)
- [🛠️ Build & Installation](#️-build--installation)
- [🎮 Usage Guide](#-usage-guide)
- [🔍 Feature Deep Dive](#-feature-deep-dive)
- [🏛️ University Structure](#️-university-structure)
- [🔒 Security & Limitations](#-security--limitations)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

---

## 🎯 Overview

UniHub CLI is a cutting-edge academic resource management system designed specifically for NIT Trichy students. It demonstrates fundamental computer science concepts through the implementation of multiple data structures working in harmony to provide efficient user management, academic hierarchy navigation, and resource sharing capabilities.

### Key Innovations
- **8+ Essential Data Structures**: AVL Trees, Binary Search Trees, Hash Tables, DAGs, Arrays, Priority Queues, Graphs, Stacks, LRU Cache
- **Hybrid Architecture**: Multiple data structures working together for optimal performance
- **Real-world Application**: Practical implementation of fundamental CS concepts
- **Educational Focus**: Designed for learning core data structures taught in college coursework

---

## 🏗️ Advanced Architecture

### Core System Components

#### 1. **UniHub Core** (`unihub_core.h`)
Central hub managing all data structures and coordinating system operations.

#### 2. **Enhanced Menu System** (`enhanced_menu.h`)
Advanced user interface with breadcrumb navigation and contextual menus.

#### 3. **Hybrid User Management** (`user_manager.h`)
- **Hash Table**: O(1) email lookup
- **AVL Tree**: Sorted user browsing
- **LRU Cache**: Recent user tracking
- **Graph**: Social connections

#### 4. **Academic Manager** (`academic_manager.h`)
- **Tree Structure**: University hierarchy
- **DAG**: Subject prerequisites
- **Hash Map**: Quick subject lookup

#### 5. **Resource Index** (`resource_index.h`)
- **Binary Search Tree**: Efficient metadata storage
- **Simple Array**: Autocomplete functionality
- **Priority Queue**: Popular resources
- **Inverted Index**: Full-text search

---

## 🚀 Key Features

### 🎓 Academic Features
- **Multi-Branch Support**: 10 engineering branches + architecture
- **Custom Curricula**: Branch-specific subject arrangements
- **Prerequisite Tracking**: DAG-based dependency management
- **Credit System**: Complete academic credit tracking
- **Teacher Assignment**: Section-specific instructor allocation

### 👥 User Management
- **Hybrid Authentication**: Multiple data structure integration
- **Profile Management**: Comprehensive user data handling
- **Social Features**: User connections and study groups
- **Recent Access**: LRU-based quick access to frequent users
- **Search Capabilities**: Prefix-based user discovery

### 📚 Resource System
- **8 Resource Types**: Notes, Assignments, PPTs, Papers, Tests, etc.
- **Advanced Search**: Multi-criteria resource discovery
- **Popularity Ranking**: Download-based resource scoring
- **Simple Auto-complete**: Array-based resource name suggestions
- **Related Resources**: Graph-based content relationships

### 🧭 Navigation
- **Breadcrumb System**: Stack-based navigation history
- **Context Awareness**: Location-sensitive operations
- **Back Navigation**: Intuitive menu traversal

---

## 📊 Data Structures Implementation

### Primary Data Structures

| Data Structure | Purpose | Location | Time Complexity |
|----------------|---------|----------|-----------------|
| **Hash Table** | User lookup by email | `user_manager.h` | O(1) average |
| **AVL Tree** | Sorted user browsing | `data_structures.h` | O(log n) |
| **Binary Search Tree** | Resource metadata storage | `resource_index.h` | O(log n) average |
| **DAG** | Subject prerequisites | `academic_manager.h` | O(V+E) |
| **Simple Array** | Autocomplete functionality | `data_structures.h` | O(n) |
| **Priority Queue** | Popular resources | `resource_index.h` | O(log n) |
| **Graph** | User/resource relationships | `data_structures.h` | O(V+E) |
| **LRU Cache** | Recent user access | `user_manager.h` | O(1) |
| **Stack** | Navigation history | `unihub_core.h` | O(1) |

### Hybrid Operations Examples

1. **User Login Flow**:
   ```
   Hash Table (email lookup) → AVL Tree (sorted display) → LRU Cache (update recent)
   ```

2. **Resource Search**:
   ```
   Simple Array (autocomplete) → BST (metadata search) → Priority Queue (popularity ranking)
   ```

3. **Subject Prerequisites**:
   ```
   DAG (prerequisite check) → Tree (academic hierarchy) → Hash Map (quick lookup)
   ```

---

## 🎓 Academic System

### Supported Branches
| Branch Code | Full Name | Years | Semesters |
|-------------|-----------|-------|-----------|
| CSE | Computer Science and Engineering | 4 | 8 |
| ECE | Electronics and Communication Engineering | 4 | 8 |
| EEE | Electrical and Electronics Engineering | 4 | 8 |
| ICE | Instrumentation and Control Engineering | 4 | 8 |
| ME | Mechanical Engineering | 4 | 8 |
| CHE | Chemical Engineering | 4 | 8 |
| PROD | Production Engineering | 4 | 8 |
| CIVIL | Civil Engineering | 4 | 8 |
| MME | Metallurgical and Materials Engineering | 4 | 8 |
| ARCH | Architecture | 5 | 10 |

### Custom Curriculum Example
**CSE Year 2, Semester 3, Section B**:
- Computer Organization (Prof. Mala) - 4 credits
- Principles of Programming Languages (Prof. Bala) - 3 credits  
- Data Structures (Prof. Oswald) - 4 credits
- Probability and Operations Research (Prof. Shivaranjini) - 3 credits
- Combinatorics and Graph Theory (Prof. Pavan) - 3 credits
- Digital Systems Design (Prof. Shameedha) - 4 credits

---

## 📁 Project Structure

```
UniHub-CLI/
├── Code/
│   ├── src/                          # Source files
│   │   ├── main.cpp                  # Entry point (Enhanced Menu)
│   │   ├── auth.cpp                  # Authentication & profiles
│   │   ├── storage.cpp               # File/directory utilities
│   │   ├── subjects.cpp              # Subject generation
│   │   ├── resources.cpp             # Resource management
│   │   └── resource_index.cpp        # B-Tree implementation
│   │
│   ├── include/                      # Header files
│   │   ├── enhanced_menu.h           # Advanced UI system
│   │   ├── unihub_core.h             # Central data hub
│   │   ├── user_manager.h            # Hybrid user management
│   │   ├── academic_manager.h        # Tree + DAG academics
│   │   ├── resource_index.h          # BST + Array + Queue system
│   │   ├── data_structures.h         # Core implementations
│   │   ├── auth.h                    # Authentication interfaces
│   │   ├── storage.h                 # Storage utilities
│   │   ├── subjects.h                # Subject management
│   │   └── resources.h               # Resource interfaces
│   │
│   ├── data/                         # Application data
│   │   ├── users/                    # User profiles & credentials
│   │   │   ├── *.profile             # User profile (CSV format)
│   │   │   └── *.cred                # User credentials (salt + hash)
│   │   └── resources/                # Hierarchical resource storage
│   │       └── {year}/{semester}/{branch}/{section}/{subject}/{type}/
│   │
│   ├── build/                        # Compiled object files
│   │   ├── auth.o
│   │   ├── main.o
│   │   ├── resource_index.o
│   │   ├── resources.o
│   │   ├── storage.o
│   │   └── subjects.o
│   │
│   ├── bin/                          # Executable
│   │   └── unihub
│   │
│   ├── .vscode/                      # VS Code configuration
│   │   └── tasks.json                # Build/run tasks
│   │
│   ├── Makefile (delegates to top-level)  # Kept for compatibility; top-level Makefile is primary
│   ├── .gitignore                    # Git ignore rules
│   └── README.md                     # Technical documentation
│
└── README.md                         # Main project documentation
```

---

## 💾 Data Storage Layout

### User Data Structure
```
data/users/
├── {email}.profile     # CSV: firstName,lastName,email,year,semester,branch,section
└── {email}.cred        # Salt (line 1) + Hash (line 2)
```

### Resource Hierarchy
```
data/resources/
└── {year}/
    └── {semester}/
        └── {branch}/
            └── {section}/
                └── {subject}/
                    ├── Notes/
                    ├── Assignments/
                    ├── PPTs/
                    ├── EndSemPapers/
                    ├── CTs/
                    ├── MidSemPapers/
                    ├── YouTubeLinks/
                    └── ReferenceBooks/
```

**Example Path**:
`data/resources/2/3/CSE/B/Data Structures/Notes/lecture_01.pdf`

---

## 🛠️ Build & Installation

### Prerequisites
- **C++17 Compatible Compiler** (GCC 7+, Clang 6+)
- **Make** build system
- **Linux/Unix Environment** (tested on Ubuntu/Debian)

### Quick Start
```bash
# Clone the repository
git clone https://github.com/Adarsh-dev-tech/UniHub-CLI.git
cd UniHub-CLI

# Build the application (run at repository root)
make

# Run the application
make run

# Clean build (if needed)
make clean && make
```

### VS Code Integration
The project includes VS Code tasks for development:
- **Build UniHub**: `Ctrl+Shift+P` → "Tasks: Run Task" → "Build UniHub"
- **Run UniHub**: `Ctrl+Shift+P` → "Tasks: Run Task" → "Run UniHub"
- **Rebuild UniHub**: Clean + Build in one command

---

## 🎮 Usage Guide

### Initial Setup
```
===== Welcome to UniHub CLI (Enhanced) =====
Hybrid Data Structure Version

1) Login
2) Register
0) Exit
Choose:
```

### Registration Process
1. **Personal Information**: First name, last name, college email
2. **Security**: Password + confirmation
3. **Academic Details**: Year (1-5), semester (1-10), branch, section (A/B)
4. **Branch Input**: Accepts both full names and codes
   - "Computer Science and Engineering" or "CSE"
   - Auto-normalization to standard codes

### Main Application Features

#### 1. **Profile Management**
- View current profile information
- Edit personal and academic details
- View subject prerequisites for current semester
- Academic progression tracking

#### 2. **Subject & Resource System**
- Browse subjects for your academic configuration
- View teacher assignments and subject codes
- Access 8 different resource types per subject
- Upload/download academic materials

#### 3. **Advanced Search**
- Global resource search across all subjects
- Autocomplete suggestions while typing
- Keyword-based content discovery
- Tag-based resource filtering

#### 4. **Popular Resources**
- Most downloaded content ranking
- Rating-based resource recommendations
- Download count tracking
- Community-driven content curation

#### 5. **User Directory**
- Browse all users (sorted alphabetically)
- View recently active users
- Search users by email prefix
- Social connection features

---

## 🔍 Feature Deep Dive

### Hybrid Data Structure Benefits

#### 1. **Multi-layered User Management**
```cpp
// Hash Table: O(1) email lookup
userIndex[email] = userRecord;

// AVL Tree: Maintains sorted order
sortedEmails.insert(email);

// LRU Cache: Tracks recent access
updateRecentAccess(email);

// Graph: Social connections
socialGraph.addConnection(user1, user2);
```

#### 2. **Intelligent Resource Discovery**
```cpp
// Simple Array: Autocomplete functionality
auto suggestions = resourceAutocomplete.getWordsWithPrefix(input);

// BST: Efficient metadata storage
bstRoot.searchAll(); // Get all resources for filtering

// Priority Queue: Popularity ranking
popularQueue.push(resourceWithHighDownloads);

// Inverted Index: Full-text search
invertedIndex[keyword].push_back(resourceFilename);
```

#### 3. **Academic Hierarchy Navigation**
```cpp
// DAG: Prerequisite management
prerequisiteGraph.getPrerequisites(subjectCode);

// Tree: University structure
universityRoot->children[branchCode]->children[year];

// Stack: Navigation history
navigationHistory.push(currentLocation);
```

### Performance Characteristics

| Operation | Data Structure | Time Complexity | Space Complexity |
|-----------|----------------|-----------------|------------------|
| User login | Hash Table | O(1) | O(n) |
| Sorted user list | AVL Tree | O(n) | O(n) |
| Resource search | BST | O(log n) average | O(n) |
| Autocomplete | Simple Array | O(n) | O(n) |
| Prerequisites | DAG | O(V + E) | O(V + E) |
| Popular resources | Priority Queue | O(log n) | O(n) |

---

## 🏛️ University Structure

### Academic Calendar
- **Engineering Branches**: 4 years (8 semesters)
- **Architecture**: 5 years (10 semesters)
- **Sections**: A and B (different faculty assignments)
- **Credit System**: 3-4 credits per subject
- **Prerequisites**: Enforced through DAG validation

### Subject Pool Examples

#### Computer Science Engineering (CSE)
- **Foundation**: Programming Fundamentals, Data Structures, Algorithms
- **Core**: Computer Networks, Operating Systems, DBMS, Software Engineering  
- **Advanced**: AI Basics, ML Intro, Compilers, Computer Organization

#### Electronics & Communication (ECE)
- **Foundation**: Circuit Theory, Digital Electronics, Signals and Systems
- **Core**: Communication Systems, Microprocessors, VLSI Basics
- **Advanced**: Embedded Systems, Antennas, DSP, Control Systems

### Resource Types (8 Categories)
1. **Notes**: Lecture notes and study materials
2. **Assignments**: Homework and project files
3. **PPTs**: Presentation slides from classes
4. **EndSemPapers**: End semester examination papers
5. **CTs**: Class test papers and solutions
6. **MidSemPapers**: Mid semester examination papers
7. **YouTubeLinks**: Educational video resources
8. **ReferenceBooks**: Textbook recommendations and digital copies

---

## 🔒 Security & Limitations

### Security Implementation
- **Password Hashing**: Custom salt + `std::hash`
- **File-based Storage**: Local credential management
- **Input Validation**: Branch normalization and sanitization
- **Access Control**: Session-based user management

### Known Limitations
⚠️ **Not Production Ready**: This is an educational/demonstration project

- **Cryptographic Security**: Uses `std::hash` which is NOT cryptographically secure
- **Concurrent Access**: No multi-user/threading support
- **Data Persistence**: File-based storage without database features
- **Network Security**: No encryption or secure communication
- **Platform Dependency**: Designed for Unix/Linux environments

### Suitable Use Cases
✅ **Educational**: Learning data structures and algorithms
✅ **Demonstration**: Showcasing hybrid system design
✅ **Prototyping**: Academic project development
✅ **Research**: Algorithm performance comparison

---

## 🤝 Contributing

### Development Guidelines
1. **Code Style**: Follow existing C++17 conventions
2. **Documentation**: Add comprehensive comments for new features
3. **Testing**: Verify functionality across different academic configurations
4. **Data Structures**: Maintain the hybrid approach for new features

### Contribution Areas
- **New Data Structures**: Implement additional algorithms
- **Performance Optimization**: Improve existing implementations
- **Feature Enhancement**: Add new academic management features
- **UI/UX Improvements**: Enhance the menu system
- **Documentation**: Improve code documentation and examples

### Getting Started
```bash
# Fork the repository
git fork https://github.com/Adarsh-dev-tech/UniHub-CLI.git

# Create a feature branch
git checkout -b feature/your-feature-name

# Make your changes and test
make clean && make
./bin/unihub

# Submit a pull request
git push origin feature/your-feature-name
```

---

## 📄 License

**MIT License** - Educational/Research Use

This project is developed for educational purposes to demonstrate advanced data structure implementations and hybrid system design. It is not intended for production use in real academic institutions.

### Usage Rights
- ✅ Educational use and learning
- ✅ Research and academic projects  
- ✅ Code study and algorithm analysis
- ✅ Modification and enhancement
- ❌ Production deployment without security improvements
- ❌ Commercial use without proper security audit

---

### 🎯 Project Goals Achieved

- [x] **Fundamental Data Structure Implementation**: 8+ essential data structures working together
- [x] **Real-world Application**: Practical academic resource management
- [x] **Educational Value**: Demonstrates core CS concepts taught in college
- [x] **Scalable Design**: Modular architecture for easy extension
- [x] **Performance Optimization**: Multiple data structures for different use cases
- [x] **User Experience**: Intuitive navigation and comprehensive features

**UniHub CLI** represents a practical approach to combining fundamental computer science concepts with application development, making it an excellent resource for learning essential data structures commonly taught in college coursework.