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

const express = require('express');
const multer = require('multer');//useful for setting up temperorary folders for server
const { exec } = require('child_process');
const path = require('path') //addition that allows for handling and transforming file paths
const app=express(); //creates a express app
const port = 5500 

/*cross-origin resource sharing (CORS) is a security
mechanism that allows or denies requests from diff 
origins */
const cors = require('cors'); 
app.use(cors());

const upload = multer({dest: 'uploads/'}); //set up uploaded files to go to uploads dir

//Serve static HTML/JS files in a 'public' folder
app.use(express.static('public'));

/*
    main logic for c++ connection, takes the web url with POST and the /huffman? addition
    with subsequent variables after ?
    Ex. 'https://localhost5500/huffman?typeConv=${encryptordecrypt}&filePath=${filePath}'

*/
app.post('/huffman', upload.single('mainfile'), (req, res)=>{
    if(!req.file){
        return res.status(400).send("No file uploaded.");
    }
    console.log("File received and saved to: ", req.file.path);
    const type = req.query.typeConv; //either 'encrypt' or 'decrypt'
    
   

});

//Setup for server to run via listening when port is opened after running: 'node server.js'
app.listen(port, ()=>{
    console.log(`Server running at http:://localhost:${port}`);
});