//Made by Andy 2025
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <regex>
#include <iomanip>
#include <sstream>

#include <cstdio> // 用于 std::rename
#include <cstdlib> // 用于 std::system

namespace fs = std::filesystem;

struct BlogPost {
    std::string title;
    std::string filename;
    std::time_t postDate;  // 改为使用文章中的日期
};

// 解析YAML front matter中的日期
std::time_t parseDateFromFrontMatter(const std::string& content) {
    std::regex dateRegex(R"(date:\s*([0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\+[0-9]{2}:[0-9]{2}))");
    std::smatch match;
    
    if (std::regex_search(content, match, dateRegex)) {
        std::tm tm = {};
        std::istringstream ss(match[1].str());
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        if (!ss.fail()) {
            return std::mktime(&tm);
        }
    }
    
    // 如果没有找到日期，返回当前时间
    return std::time(nullptr);
}

// 解析标题
std::string parseTitleFromFrontMatter(const std::string& content) {
    std::regex titleRegex(R"(title:\s*(.*))");
    std::smatch match;
    
    if (std::regex_search(content, match, titleRegex)) {
        return match[1].str();
    }
    
    return "Untitled Post";
}

// 提取Markdown内容（去除front matter）
std::string extractMarkdownContent(const std::string& content) {
    size_t frontMatterEnd = content.find("---", 3); // 跳过第一个---
    if (frontMatterEnd != std::string::npos) {
        return content.substr(frontMatterEnd + 3);
    }
    return content;
}

// 极简Markdown转HTML
std::string markdownToHtml(const std::string& markdown) {
    std::string html;
    bool inCodeBlock = false;
    
    for (size_t i = 0; i < markdown.size(); ) {
        if (!inCodeBlock && markdown.substr(i, 3) == "```") {
            html += "<pre><code>";
            inCodeBlock = true;
            i += 3;
        } else if (inCodeBlock && markdown.substr(i, 3) == "```") {
            html += "</code></pre>";
            inCodeBlock = false;
            i += 3;
        } else if (!inCodeBlock && markdown.substr(i, 2) == "# ") {
            html += "<h1>";
            i += 2;
            while (i < markdown.size() && markdown[i] != '\n') {
                html += markdown[i++];
            }
            html += "</h1>";
        } else if (!inCodeBlock && markdown.substr(i, 3) == "## ") {
            html += "<h2>";
            i += 3;
            while (i < markdown.size() && markdown[i] != '\n') {
                html += markdown[i++];
            }
            html += "</h2>";

        } else if (!inCodeBlock && markdown.substr(i, 4) == "### ") {
            html += "<h3>";
            i += 4;
            while (i < markdown.size() && markdown[i] != '\n') {
                html += markdown[i++];
            }
            html += "</h3>";

        } else if (!inCodeBlock && markdown.substr(i, 2) == "- ") {
            html += "<ul><li>";
            i += 2;
            while (i < markdown.size() && markdown[i] != '\n') {
                html += markdown[i++];
            }
            html += "</li></ul>";
        } else if (!inCodeBlock && markdown.substr(i, 2) == "**") {
            html += "<strong>";
            i += 2;
            while (i < markdown.size() && markdown.substr(i, 2) != "**") {
                html += markdown[i++];
            }
            html += "</strong>";
            i += 2;
        } else if (!inCodeBlock && markdown[i] == '[') {
            // 简单链接处理
            size_t linkEnd = markdown.find(']', i);
            size_t urlStart = markdown.find('(', linkEnd);
            size_t urlEnd = markdown.find(')', urlStart);
            
            if (linkEnd != std::string::npos && urlStart != std::string::npos && urlEnd != std::string::npos) {
                std::string text = markdown.substr(i + 1, linkEnd - i - 1);
                std::string url = markdown.substr(urlStart + 1, urlEnd - urlStart - 1);
                html += "<a href=\"" + url + "\">" + text + "</a>";
                i = urlEnd + 1;
                continue;
            }
            
            html += markdown[i++];
        } else {
            html += markdown[i++];
        }
    }
    
    return html;
}

// 生成HTML页面
void generateHtml(const std::string& title, const std::string& postDatenext, const std::string& content, const std::string& filename) {
    std::ofstream out(filename);
    out << "<!DOCTYPE html>\n";
    out << "<html>\n";
    out << "<head>\n";
    out << "<meta charset=\"UTF-8\">\n";
    out << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    out << "<title>" << title << "</title>\n";
    /*
    out << "<style>\n";
    out << "body {\n";
    out << "    transition: all 0.3s ease;\n";
    out << "    font-family: Arial, sans-serif;\n"; 
    out << "    line-height: 1.6;\n"; 
    out << "    max-width: 800px;\n"; 
    out << "    margin: 0 auto;\n"; 
    out << "    padding: 20px;\n";
    out << "}\n";
    out << "h1 { color: #2c3e50; }\n";
    out << "a { color: #3498db; text-decoration: none; }\n";
    out << "a:hover { text-decoration: underline; }\n";
    out << ".post-list { list-style: none; padding: 0; }\n";
    out << ".post-item { margin-bottom: 15px; }\n";
    out << ".post-date { color: #7f8c8d; font-size: 0.9em; }\n";
    out << ".dark { background: #121212; color: #e0e0e0; }\n";
    out << ".dark h1 { color: #f5f5f5; }\n";
    out << ".dark a { color:rgb(52, 152, 219); }\n";
    out << ".dark .post-date { color: #b3b3b3; }\n";
    */
    out << "<link rel=\"stylesheet\" href=\"styles.css\">\n";
    /*
    out << "<style>\n";
    out << "body { font-family: Arial, sans-serif; line-height: 1.6; max-width: 800px; margin: 0 auto; padding: 20px; }\n";
    out << "h1 { color: #2c3e50; }\n";
    out << "pre { background: #f8f8f8; padding: 10px; }\n";
    out << "a { color: #3498db; text-decoration: none; }\n";
    out << "a:hover { text-decoration: underline; }\n";
    out << ".post-list { list-style: none; padding: 0; }\n";
    out << ".post-item { margin-bottom: 15px; }\n";
    out << ".post-date { color: #7f8c8d; font-size: 0.9em; }\n";
    */
    out << "</style>\n";
    out << "<script type=\"module\" src=\"dark.js\"></script>\n";
    out << "</head>\n";
    out << "<body>\n";
    out << "<h1>" << title << "</h1>\n";
    out << "<br>";
    out << "<br>";
    out << "<button onclick=\"document.body.classList.toggle('dark')\">Change theme</button>\n";
    out << "<br>";
    out << "<br>\n";
    out << postDatenext << "\n";
    out << "<br>";
    out << content << "\n";
    out << "</body>\n";
    out << "</html>\n";
}

// 生成索引页面
/*
void generateIndex(const std::vector<BlogPost>& posts) {
    std::ofstream out("output/index.html");
    out << "<!DOCTYPE html>\n";
    out << "<html>\n";
    out << "<head>\n";
    out << "<meta charset=\"UTF-8\">\n";
    out << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    out << "<title>Andy's Blog</title>\n";
    

    out << "<style>\n";
    out << "body { font-family: Arial, sans-serif; line-height: 1.6; max-width: 800px; margin: 0 auto; padding: 20px; }\n";
    out << "h1 { color: #2c3e50; }\n";
    out << "a { color: #3498db; text-decoration: none; }\n";
    out << "a:hover { text-decoration: underline; }\n";
    out << ".post-list { list-style: none; padding: 0; }\n";
    out << ".post-item { margin-bottom: 15px; }\n";
    out << ".post-date { color: #7f8c8d; font-size: 0.9em; }\n";
    out << "</style>\n";
    */
/*
    out << "<style>\n";
    out << "body {\n";
    out << "    transition: all 0.3s ease;\n";
    out << "    font-family: Arial, sans-serif;\n"; 
    out << "    line-height: 1.6;\n"; 
    out << "    max-width: 800px;\n"; 
    out << "    margin: 0 auto;\n"; 
    out << "    padding: 20px;\n";
    out << "}\n";
    out << "h1 { color: #2c3e50; }\n";
    out << "a { color: #3498db; text-decoration: none; }\n";
    out << "a:hover { text-decoration: underline; }\n";
    out << ".post-list { list-style: none; padding: 0; }\n";
    out << ".post-item { margin-bottom: 15px; }\n";
    out << ".post-date { color: #7f8c8d; font-size: 0.9em; }\n";
    out << ".dark { background: #121212; color: #e0e0e0; }\n";
    out << ".dark h1 { color: #f5f5f5; }\n";
    out << ".dark a { color:rgb(52, 152, 219); }\n";
    out << ".dark .post-date { color: #b3b3b3; }\n";
    out << "</style>\n";


    out << "</head>\n";
    out << "<body>\n";
    out << "<h1>Andy's Blog</h1>\n";
    out << "<p style=\"font-family: 'Times New Roman', serif;\">Stay hungry, stay foolish</p>" ;

    out << "<button onclick=\"document.body.classList.toggle('dark')\">Change theme</button>\n";


    out << "<ul class=\"post-list\">\n";
    
    for (const auto& post : posts) {
        char dateStr[100];
        std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", std::localtime(&post.postDate));
        
        out << "<li class=\"post-item\">\n";
        out << "<a href=\"" << post.filename << "\">" << post.title << "</a>\n";
        out << "<div class=\"post-date\">" << dateStr << "</div>\n";
        out << "</li>\n";
    }
    
    out << "</ul>\n";
    out << "</body>\n";
    out << "©2019-2025 <a href=\"http://andychen.net/\">Andy</a>" ;
    out << "</html>\n";
}
*/
void generateIndex(const std::vector<BlogPost>& posts) {
    std::ofstream out("output/index.html");
    out << "<!DOCTYPE html>\n";
    out << "<html>\n";
    out << "<head>\n";
    out << "<meta charset=\"UTF-8\">\n";
    out << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    out << "<title>Andy's Blog</title>\n";

    out << "<link rel=\"stylesheet\" href=\"styles.css\">\n";

    /*
    out << "<style>\n";
    out << "body {\n";
    out << "    transition: all 0.3s ease;\n";
    out << "    font-family: Arial, sans-serif;\n"; 
    out << "    line-height: 1.6;\n"; 
    out << "    max-width: 800px;\n"; 
    out << "    margin: 0 auto;\n"; 
    out << "    padding: 20px;\n";
    out << "}\n";
    out << "h1 { color: #2c3e50; }\n";
    out << "a { color: #3498db; text-decoration: none; }\n";
    out << "a:hover { text-decoration: underline; }\n";
    out << ".post-list { list-style: none; padding: 0; }\n";
    out << ".post-item { margin-bottom: 15px; }\n";
    out << ".post-date { color: #7f8c8d; font-size: 0.9em; }\n";
    out << ".dark { background: #121212; color: #e0e0e0; }\n";
    out << ".dark h1 { color: #f5f5f5; }\n";
    out << ".dark a { color:rgb(52, 152, 219); }\n";
    out << ".dark .post-date { color: #b3b3b3; }\n";


    out << "#loadMore {\n";
    out << "    display: block;\n";
    out << "    margin: 20px auto;\n";
    out << "    padding: 10px 20px;\n";
    out << "    background: #3498db;\n";
    out << "    color: white;\n";
    out << "    border: none;\n";
    out << "    border-radius: 4px;\n";
    out << "    cursor: pointer;\n";
    out << "}\n";
    out << "#loadMore:hover { background: #2980b9; }\n";
    out << ".hidden-post { display: none; }\n";
    out << "</style>\n";

    */

    out << "<script type=\"module\" src=\"dark.js\"></script>\n";
    out << "</head>\n";
    out << "<body>\n";
    out << "<h1>Andy's Blog</h1>\n";
    out << "<p style=\"font-family: 'Times New Roman', serif;\">Stay hungry, stay foolish</p>\n";
    out << "<button onclick=\"document.body.classList.toggle('dark')\">Change theme</button>\n";
    out << "<ul class=\"post-list\" id=\"postList\">\n";
    
    // 显示前10篇文章，其余隐藏
    for (size_t i = 0; i < posts.size(); ++i) {
        char dateStr[150];
        std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", std::localtime(&posts[i].postDate));
        
        out << "<li class=\"post-item" << (i >= 15 ? " hidden-post" : "") << "\">\n";
        out << "<a href=\"" << posts[i].filename << "\">" << posts[i].title << "</a>\n";
        out << "<div class=\"post-date\">" << dateStr << "</div>\n";
        out << "</li>\n";
    }
    
    out << "</ul>\n";
    
    // 如果文章超过10篇，添加"加载更多"按钮
    if (posts.size() > 15) {
        out << "<button id=\"loadMore\">Load More</button>\n";
        out << "<script>\n";
        out << "let visiblePosts = 15;\n";
        out << "const posts = document.querySelectorAll('.post-item');\n";
        out << "const loadMoreBtn = document.getElementById('loadMore');\n";
        out << "\n";
        out << "loadMoreBtn.addEventListener('click', () => {\n";
        out << "    visiblePosts += 15;\n";
        out << "    for (let i = 0; i < posts.length && i < visiblePosts; i++) {\n";
        out << "        posts[i].classList.remove('hidden-post');\n";
        out << "    }\n";
        out << "    if (visiblePosts >= posts.length) {\n";
        out << "        loadMoreBtn.style.display = 'none';\n";
        out << "    }\n";
        out << "});\n";
        out << "</script>\n";
    }
    
    out << "©2019-2025  <a href=\"http://andychen.net/\">Andy</a>\n";
    out << "</body>\n";
    out << "</html>\n";
}

std::string timeToString(std::time_t time) {
                  std::tm tm = *std::localtime(&time); // 转换为本地时间
                  char buffer[80];
                  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
                  return buffer;
}

int main() {
    try {
        // 创建输出目录
        fs::create_directory("output");
        
        std::vector<BlogPost> posts;



        if (fs::exists("styles.css")) {
            fs::copy("styles.css", "output/styles.css", fs::copy_options::overwrite_existing);
            std::cout << "Copied: styles.css -> output/styles.css" << std::endl;
        } else {
            std::cerr << "Warning: styles.css not found in current directory" << std::endl;
        }

        if (fs::exists("dark.js")) {
            fs::copy("dark.js", "output/dark.js", fs::copy_options::overwrite_existing);
            std::cout << "Copied: dark.js -> output/dark.js" << std::endl;
        } else {
            std::cerr << "Warning: dark.js not found in current directory" << std::endl;
        }


        
        // 遍历posts目录下的所有.md文件
        for (const auto& entry : fs::directory_iterator("posts")) {
            if (entry.path().extension() == ".md") {
                // 读取Markdown文件
                std::ifstream in(entry.path());
                std::string content((std::istreambuf_iterator<char>(in)), 
                                  std::istreambuf_iterator<char>());
                
                // 解析front matter
                std::string title = parseTitleFromFrontMatter(content);
                std::time_t postDate = parseDateFromFrontMatter(content);
                std::string markdownContent = extractMarkdownContent(content);
                
                // 生成HTML文件名
                std::string htmlFilename = entry.path().stem().string() + ".html";


                std::string postDatenext = timeToString(postDate);
                
                // 转换并生成HTML
                generateHtml(title, postDatenext,
                            markdownToHtml(markdownContent), 
                            "output/" + htmlFilename);
                
                // 保存文章信息用于生成索引
                posts.push_back({
                    title,
                    htmlFilename,
                    postDate
                });
                
                std::cout << "Generated: " << htmlFilename << std::endl;
            }
        }
        
        // 按文章日期排序（最新的在前）
        std::sort(posts.begin(), posts.end(), [](const BlogPost& a, const BlogPost& b) {
            return a.postDate > b.postDate;
        });
        
        // 生成索引页面
        generateIndex(posts);
        std::cout << "Generated: index.html" << std::endl;
        
        std::cout << "Blog generation completed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}