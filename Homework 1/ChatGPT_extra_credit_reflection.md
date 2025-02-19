**Dylan Phoutthavong**

**February 1st, 2025**

**CSCI 3751**

# Reflection on AI Usage in Homework #1
For this assignment, my main approach was to complete the homework independently without relying on AI tools. I used foundational knowledge of Unix commands and used additional resources, such as Google, when I could not find a certain bash script. However, there were specific instances where I used ChatGPT to enhance my understanding and problem-solving skills, especially for prompts that required multiple actions within a single bash script line.
 
# How I Used Generative AI to Support My Work
I used ChatGPT in a few prompts:
1.	**Complex Commands**: For tasks that involved chaining multiple actions (ex. unzipping and untarring a file in one command), I used ChatGPT to better understand the syntax and find different variations of the command. This helped me ensure accuracy and efficiency.
2.	**Exploration Beyond the Assignment**: To deepen my understanding, I asked ChatGPT to explain alternative methods for some commands, such as using `awk` or `sed` for file processing. For example, I learned how to use `awk` to count lines in a file without using on `wc -l`.
3.	**Command Optimizatio**: I experimented with ChatGPT’s suggestions to combine commands, such as using pipes (`|`) and redirections (`>`), to simplify and optimize workflow. This allowed me to practice advanced techniques that go beyond basic commands.
 
# Impact of AI on My Understanding
Using ChatGPT enhanced my learning by:
- Clarifying difficult concepts, such as symbolic links and inode details.
- Helped me to explore beyond the assignment, which improved my understanding of Unix.
- Introducing me to alternative approaches to tasks, which deepened my knowledge and improved my problem-solving skills.
 
### Steps to Verify AI-Generated Information
To ensure accuracy and reliability:
1.	I tested all solutions ChatGPt provided intto the Unix terminal.
2.	I cross-referenced ChatGPT’s solution with other online resources found on google.
3.	I only used solutions that I fully understood and could explain independently.

### Demonstrating Depth of Understanding with Unix Commands
1. **Advanced File Searching and Processing**:

- Outside the required use of the find command, I explored using grep and xargs to execute commands on the files that matched specific patterns. For example:
```
find . -type f -name "*.txt" | xargs grep "keyword"
```
This allowed me to search for a keyword across multiple files simultaneously.

1. **Command Optimization with Pipes**:

- Instead of performing tasks sequentially, I combined multiple commands using pipes. For example, I used:
```
tar -xzvf src.3e.tar.gz | grep ".c$"
```
to extract files while simultaneously filtering and listing all `.c` files.

3. **Automation with Shell Scripts**:

- To streamline repetitive tasks, I created a small shell script that:
  - Created directories.
  - Moved files to specific locations.
  - Generated logs with timestamps.
- Example:
```
#!/bin/bash
mkdir -p csci3751/logs
cp /var/tmp/CSCI3751/src.3e.tar.gz csci3751/
cd csci3751
tar -xzvf src.3e.tar.gz > logs/extraction.log
date >> logs/extraction.log
```

4. **Exploration of Hidden Features**:

- I experimented with lesser-known ls options like --time-style to customize the display of file timestamps:
```
ls -l --time-style=long-iso
```

5. **Deep Dive into Symbolic and Hard Links**:

- I created both hard and symbolic links to observe their differences in behavior:
```
ln original.txt hardlink.txt
ln -s original.txt symlink.txt
```
- Then, I examined how modifying or deleting the original file impacted the links.
  
### Creative Problem-Solving and AI Usage
While using resouces in class and Google for most tasks, I used ChatGPT to:

1. Clarify the behavior of challenging commands and flags.
2. Learn about advanced Unix utilities like `sed` for inline file editing.
   
For example, to append the line count to a file in one command, I learned to use:

```
wc -l find-result | awk '{print $1}' >> find-result
```

### Reflection on Learning
The extra credit exploration encouraged me to:

1. Experiment with command combinations and scripting to simplify workflows.
2. Understand the broader applications of Unix tools, which are critical for real-world problem-solving.
3. Appreciate the value of AI as a learning enhancer when used responsibly.
   
Through this deeper dive into Unix, I not only completed the assignment but also gained more knowledge in handling advanced tasks efficiently and creatively.
