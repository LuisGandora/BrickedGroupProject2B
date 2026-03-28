<h1>BrickedGroupProject2B: CSV Huffman Style Encryption</h1>

Youtube Link:https://youtu.be/vz4rwuufSoU

Link to Documentation: https://docs.google.com/document/d/17a82JZqoJ766iOT9wh9yuCjJN4BrCI0Yj62xOCCXOWs/edit?usp=sharing

<h2>Instructions to Run</h2>

1. clone this repo with 'git clone https://github.com/LuisGandora/BrickedGroupProject2B.git'

Ensure you are in the directory/folder where you cloned this for the next few steps

2. Go into public with 'cd public'

3. create the executable via: 'g++ -o out.exe main.cpp 'Huffman Tree .h'

4. go back into main directory via 'cd ..'

5. run server with 'node public\server.js' then visit the local link established

<h2>Features</h2>
-Locally hosted website that works as the front end for encryption types (C++/node.js)
-Huffman coding based Encryption and decryption of spreadsheets inputted
-History Side Tab to refer to the  3 most accessed files for reference

<h2>Data</h2>
 Our main data sheet is a 100,000 data point set about proxy student academic records

<h2>Tools used</h2>
-Languages: C++, JS, HTML, CSS
-Tools: VScode, Node.js
-Libraries: iostream, fstream, vector, unordered_map, queue, exception_ptr, functional, algorithm, express, multer, child_process, path, fs


<h2>Data Structures</h2>
-Huffman Tree: Data Structure used to encrypt and decrypt spreadsheets
	--Use of unordered map for storing frequency map and codes
	--Use of custom priority queue for storing the Node structure
	--Used for encryption and decryption of spreadsheets
-LRU Cache: Data structure used for history sidebar

<h2>Distribution of Responsibilities and Roles</h2>
-Luis: Front end node.js website and LRU Cache based history
-Reg: C++ Huffman Tree class with methods for encryption and decryption plus documentation
-Adam: I/O of main C++ executable and documentation




