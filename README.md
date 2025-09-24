# 🎵 Music Library Management System (C++ OOP Project)

This project is a **menu-driven C++ program** that demonstrates **all major Object-Oriented Programming (OOP) and advanced C++ concepts**.  
It implements a small **Music Library Management System** where you can add albums, add songs, display a library, search songs, combine albums, use templates, handle exceptions, and save/load data from files.

---

## 🚀 Features
- Add Albums and Songs dynamically  
- Display full Music Library with albums and songs  
- Search songs longer than a given duration  
- Scale (modify) song duration using the `this` pointer  
- Combine songs from different albums using **operator overloading**  
- Demonstrate **multiple inheritance** (`Media` + `Exportable`)  
- Swap songs using **function template**  
- Search songs using **class template**  
- Save and load library data using **file handling**  
- Exception handling for invalid inputs, album/library limits, etc.

---

## 🛠️ Concepts Covered
This project implements **all 12 major concepts from advanced C++**:

1. **Basics of C++**: libraries, variables, data input/output (`cin`, `cout`)  
2. **Structures**: `Artist` struct with members and a display function  
3. **Classes, Objects & Function Overloading**: `Song` class with multiple display functions  
4. **Dynamic Memory Allocation & Array of Objects**: Albums and Library store data dynamically  
5. **Constructors & Destructor**: Default, parameterized, copy constructor, and destructors in `Song`, `Album`, and `MusicLibrary`  
6. **Friend Function & `this` Pointer**: Friend function to access private data, `this` pointer in scaling duration  
7. **Const & Static Members/Functions**: Static counters, const members, and const functions  
8. **Inheritance**: Single (`Song : Media`), Multilevel (`Album : Song`), Multiple (`PlayableItem : Media, Exportable`)  
9. **Operator Overloading**: Overloaded `+` and `<<` operators in `Song`  
10. **Templates**: Function template (`swapItems`) and Class template (`Container<T>`)  
11. **Exception Handling**: `invalid_argument`, `overflow_error`, `out_of_range`, `runtime_error` with try-catch blocks  
12. **File Handling**: Save and load music library to/from `library.txt`

---

## 📂 Project Structure
