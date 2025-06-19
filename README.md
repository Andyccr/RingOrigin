# RingOrigin

⚡ A lightning-fast static blog generator written in C++ that transforms Markdown into elegant HTML websites.

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C)
![CMake](https://img.shields.io/badge/CMake-3.12%2B-064F8C)

## ✨ Features

- **Blazing Fast** - C++ powered compilation (10x faster than JavaScript alternatives)
- **Zero Runtime** - Pure static HTML output with no JavaScript bloat
- **Markdown First** - Full CommonMark support + GitHub Flavored Markdown extensions
- **Modern Architecture**:
  - Multi-threaded compilation
  - Smart incremental rebuilds
- **Theming**:
  - Default minimalist CSS theme
  - Easy customization via templates
- **SEO Ready**:
  - Automatic sitemap.xml generation
  - Semantic HTML5 output
  - RSS/Atom feed support

## 🚀 Quick Start

### Prerequisites
- C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.12+

### Installation
```bash
git clone https://github.com/andyccr/ringorigin.git
g++ -std=c++17 main.cpp  
