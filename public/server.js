//middleware setup between main index and server
//npm init -y
//npm install express cors
//npm install express multer
/*
    How to run:
        -Used live serve for index.html but you can also open a new terminal and run index.html i thinkg
        -Then run node server.js (ithink)
    Express is a application within node.js
    that allows for basic API connection, in this
    project, it will be used to connect the app.js to the 
    server.js so our html website locally will be able to interact
    with html
*/


//Class for LRUCache and Node management, switched from C++ to JS for better management of storage
class Node {
    constructor(data, value){
        this.data = data; //string value for filepath
        this.value = value;
        this.prev = null;
        this.next = null;
    }
}
class LRUCache{
    constructor(max_cap){
        this.max_cap = max_cap;
        this.head = new Node(null, null);
        this.tail = new Node(null, null);
        this.head.next = this.tail; //1-based list where the first node is null
        this.tail.prev = this.head;
        this.hash_map = new Map(); //takes key and string 
    }
    _remove(node){
        let prev = node.prev;
        let next = node.next;
        prev.next = next;
        next.prev = prev;
        // Put the node out of the list
    }
    _add(node){
        let next = this.head.next;
        this.head.next = node;
        node.prev = this.head;
        node.next = next;
        next.prev = node;
        //Add to front of the list
    }
    //Key is the number of files inputted
    get(key)
    {
        if(!this.hash_map.has(key))
        {
            return -1;
        }
        let node = this.hash_map.get(key);
        //remove and add node to front 
        this._remove(node);
        this._add(node);
    }
    //input number of files inputted with a subsequent filepath value
    put(key, value){
        if(this.hash_map.has(key)){
            this._remove(this.hash_map.get(key));
        }
        let hollowPurple = new Node(key, value);
        this._add(hollowPurple);
        this.hash_map.set(key, hollowPurple);
        if(this.hash_map.size > this.max_cap)
        {
            let lru = this.tail.prev;
            console.log(this.hash_map.delete(lru.data));
            this._remove(lru);
        }
    }
    accMap(){ //returns a map
        return this.hash_map;
    }
}

const express = require('express');
const multer = require('multer');//useful for setting up temperorary folders for server
const { exec } = require('child_process');
const fs = require('fs'); //use the fs library to access this
const path = require('path') //addition that allows for handling and transforming file paths
const app=express(); //creates a express app
const port = 5500;

const uploadDir = 'uploads/'; //for cleanup
const publicUploadDir = path.join(__dirname, 'uploads');

//create the fs paths for server to access
if(!fs.existsSync(uploadDir)) fs.mkdirSync(uploadDir, {recursive: true});
if(!fs.existsSync(publicUploadDir)) fs.mkdirSync(publicUploadDir, {recursive: true});

/*cross-origin resource sharing (CORS) is a security
mechanism that allows or denies requests from diff 
origins */
const cors = require('cors'); 
app.use(cors());

//Configure multer for storage into public for files to be returned as
const storage = multer.diskStorage({
    destination:(req, file, cb) =>{ //destination path for uploaded files
        cb(null, 'uploads/'); //destination path
    },
    filename: (req,file,cb) => {
        cb(null, file.originalname);
    }
});

const upload = multer({storage: storage}); //set up uploaded files stored

//Serve static HTML/JS files in a 'public' folder
app.use(express.static('public'));

//LRU Cache setup
let lru = new LRUCache(3); //store max of 3 filepaths
let fileAmnt = 0;

//Destructors to handle deleting user files inputted
async function emptyDirectory(dirPath){
    try{
        if(fs.existsSync){
            const files = await fs.promises.readdir(dirPath);

            for(const file of files){
                await fs.promises.unlink(path.join(dirPath, file));
            }
            console.log(`[Cleanup] Successfully emptied ${dirPath}`);
        }
    }
    catch(err){
        console.error(`[Cleanup] Error emptying ${dirPath}: `, err);
    }
}

async function serverDestructor(){
    console.log('\n[Server] Shutdown signal recieved. Running cleanup...');

    await emptyDirectory(uploadDir);
    await emptyDirectory(publicUploadDir);
    console.log('[Server] Cleanup complete. Exiting now.');
    process.exit(0);// exit without errors
}


/*
    main logic for c++ connection, takes the web url with POST and the /huffman? addition
    with subsequent variables after ?
    Ex. 'https://localhost5500/huffman?typeConv=${encryptordecrypt}&filePath=${filePath}'

*/
app.post('/huffman', upload.single('mainfile'), async (req, res)=>{
    try{
        if(!req.file){
            return res.status(400).send("No file uploaded.");
        }
        const type = req.query.typeConv; //either 'encrypt' or 'decrypt'
        
        console.log(`running C++ with: type=${type}, path=${req.file.path}`);

        //how to send back info to app.js for html to download in future when we finish jss

        const srcPath = req.file.path;
        

        //copy 

        //later replace with the file path of the modified code
        fileAmnt = fileAmnt+1;

        //later you can edit the destination path to point to the file edited by the 
        //executable from c++ and then store it via fs.promises.copyfile
        //store the file in req.file.path
        const destinationPath = path.join(publicUploadDir, req.file.filename);
        const realPath = path.join('uploads', req.file.filename)

        await fs.promises.copyFile(srcPath, destinationPath);
        lru.put(fileAmnt, realPath);//put the file path
        console.log(realPath);
        let lruMap = lru.accMap();
        //input as a json, 
        let response = {
            "currFilePath" : realPath,
            "size": lruMap.size,
            "data": [],
        };
        //get each element in the lruMap
        lruMap.forEach((value, key)=>{
            let new_items = [key, value.value];
            response.data = [...response.data, ...new_items];
            console.log(response);
        });
        res.status(200).json(response);
    }
    catch(error)
    {
        console.error('Error copying file: ', error);
        res.status(500).send('ERror processing file');
    }
});

//Access a specific element of the map of filepaths in LRU cache and put it on top priority after accessing
app.post('/accessMap', (req,res)=>{
    res.setHeader('Content-Type', 'text/plain');
    lru.get(res.num);//assume that num is inputted and reorder
    let lruMap = lru.accMap();
    res.write(lruMap.size); //write how many elements are in lruMap
    for(let i =0; i < lruMap.size; i++)
    {
        res.write(lruMap[i]); //write each element
    }
    res.end();
});

//Setup for server to run via listening when port is opened after running: 'node server.js'
app.listen(port, ()=>{
    console.log(`Server running at http:://localhost:${port}`);
});

//process listeners- Activate when server is shutdown.commands

// for Ctrl+C server shutdown case
process.on('SIGINT', serverDestructor);

//For server destruction?
process.on('SIGTERM', serverDestructor);



