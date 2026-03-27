//variables
const encryptDropZone = document.getElementById('Encryption');
const decryptDropZone = document.getElementById('Decryption');
const encryptInp = document.getElementById('eFileDrop');
const decryptInp = document.getElementById('dFileDrop');
//access hidden div
const hiddenDivBut = document.getElementById('hiddenHuffman');
let encryptFile=null;
let decryptFile = null;

//access sidebar div
const sideBarDiv = document.getElementById("mySide");

//Functions for sidebars
function w3_open()
{
    document.getElementById("mySide").style.width = "100%";
    document.getElementById("mySide").style.display = "block";
}

function w3_close()
{
    document.getElementById("mySide").style.display = "none";
}

async function accessKey(key) //to Call upon button press
{
    try{
        /*Fetch fetches a express link that is live via node, 
        this method allows us to pass info directly to server.js to run c++
        Uses the POST method which sends Data to backend server in the body of the request
        */
        console.log(`This is key ${key}`);
        const response = await fetch(`http://localhost:5500/accessMap?key=${key}`, {
            method:'POST',
        });
        if(response.ok){
            
            const data =  await response.json();
            console.log(`Success! access key at ${data.data}`);
            // this means that whatever C++ returns, it needs to be written in public
            hiddenInTheLeaf(data.currFilePath); // replace with file you want to use in the future
            generateSideBad(data.data);
            console.log(data);
        }
        else
        {
            console.error("Server Error: Error with data transfer for encryption, are you sure the server is on?");
            alert("Server Error: Error with data transfer for encryption, are you sure the server is on  or you inputted the right file type?");
        }

    }
    catch(err)
    {
        console.error(err.message);
        alert("Network error occured: are you sure the server.js is on?");
    }

}

//Function to take file
function handleEFile(file){
    encryptFile = file;
    //add an indixator to the css class it has been completed
    console.log("Success! File stored in var", encryptFile);
}

function handleDFile(file){
    decryptFile = file;
    
    //add an indixator to the css class it has been completed
    console.log("Success! File stored in var", decryptFile);
}

function hiddenInTheLeaf(filePath)//just unhides the button
{
    if(hiddenDivBut.classList.contains("hidden"))
    {
        hiddenDivBut.classList.remove("hidden");
        
    }
    const fileName = filePath.split('\\').pop().split('/').pop();

    let webUrl = filePath;
    if(webUrl.includes('public'))
    {
        webUrl = webUrl.split('public')[1];
    }
    webUrl = webUrl.replace(/\\/g, '/'); //replace window backslashes with standard forward slashes for web

    const anchor = hiddenDivBut.getElementsByTagName('a');
    anchor[0].setAttribute("href",webUrl); // replace code.png with the file you need to upload ig
    anchor[0].setAttribute("download",fileName);
    anchor[0].textContent = `Download ${fileName} here!`;
}

//Edit this later to connect to an onclick to change the file selection in hidden with that
function generateSideBad(data)
{
    //Delete all elements in sideBarDiv if exist
    const allElements = sideBarDiv.querySelectorAll('button'); //to prevent class erorr with dynamic shifting
    allElements.forEach(function(item){
        if(item.classList.length == 0)//temp fix for error
        {
            item.remove();
        }
        
    });
    for(let i = 0; i < data.length-1; i+=2)
    {
        if(data[i] != null)
        {
            const elementNode = document.createElement('button');
            // elementNode.setAttribute("href", data[i+1]);
            // elementNode.setAttribute("download", data[i]);
            elementNode.setAttribute("id", "LRUbutton");
            elementNode.onclick= () => accessKey(data[i]);//sets equal to key //Fix this to be strictly on click
            elementNode.textContent = `${data[i]}:${data[i+1]}`;
            elementNode.style.display = "block";
            elementNode.style.marginBottom = "10px";
            sideBarDiv.append(elementNode);
        }
        
    }
}

//make a detector for when each element (encryptDrop or decryptDrop)
//has an item over it
encryptDropZone.addEventListener('dragover', (e)=>
{
    e.preventDefault();//preven reload
    encryptDropZone.classList.add('dragover');//highlight box
});

encryptDropZone.addEventListener('dragleave', (e)=>
{
    encryptDropZone.classList.remove('dragover');//highlight box
});

encryptDropZone.addEventListener('drop', (e)=>{
    e.preventDefault();
    encryptDropZone.classList.remove('dragover');
    if(e.dataTransfer.files.length>0)
    {
        const checkfile = e.dataTransfer.files[0].name.split(".");
        if(checkfile[checkfile.length-1] !== "csv")
        {
            alert("File inputted is not of a supported type (csv)");
            return;
        }
        handleEFile(e.dataTransfer.files[0]);
        encryptInp.files = e.dataTransfer.files;
        e.preventDefault();
     
    }
});

encryptInp.addEventListener('change',(e)=>{
    if(e.target.files.length>0)
    {
        const checkfile = e.target.files[0].name.split(".");
        if(checkfile[checkfile.length-1] !== "csv")
        {
            alert("File inputted is not of a supported type (csv)");
            e.target.value = '';
            return;
        }
        handleEFile(e.target.files[0]);
    }
});


decryptDropZone.addEventListener('dragover', (e)=>
{
    
    e.preventDefault();//preven reload
    decryptDropZone.classList.add('dragover');//highlight box
});

decryptDropZone.addEventListener('dragleave', (e)=>
{
    decryptDropZone.classList.remove('dragover');//highlight box
});

decryptDropZone.addEventListener('drop', (e)=>{
    e.preventDefault();
    decryptDropZone.classList.remove('dragover');
    if(e.dataTransfer.files.length>0)
    {
        const checkfile = e.dataTransfer.files[0].name.split(".");
        if(checkfile[checkfile.length-1] !== "txt")
        {
            alert("File inputted is not of a supported type (txt)");
            return;
        }
        handleDFile(e.dataTransfer.files[0]);
        decryptInp.files = e.dataTransfer.files;
        e.preventDefault();
     
    }
});

decryptInp.addEventListener('change',(e)=>{
    if(e.target.files.length>0)
    {
        const checkfile = e.target.files[0].name.split(".");
        if(checkfile[checkfile.length-1] !== "txt")
        {
            alert("File inputted is not of a supported type (txt)");
            e.target.value = '';
            return;
        }
        handleDFile(e.target.files[0]);
    }
});

//Handle requests to clear the files uploaded Later!!

//Buttons for encoding and decoding
document.getElementById('encodeBut').addEventListener('click', async (e)=>{
    e.preventDefault();
    if(encryptFile == null)
    {
        alert("Please upload a file first before attempring to encode!");
        return;
    }
    //rest of logic for accessing server
    console.log("Preparing to send:", encryptFile.name);
    const formData = new FormData();//attempt to package the file for use
    formData.append('mainfile', encryptFile);

    try{
        /*Fetch fetches a express link that is live via node, 
        this method allows us to pass info directly to server.js to run c++
        Uses the POST method which sends Data to backend server in the body of the request
        */
        const response = await fetch(`http://localhost:5500/huffman?typeConv=${"encrypt"}`, {
            method:'POST',
            body: formData
        });
        if(response.ok){
            console.log("Success! File sent for encryption");
            const data =  await response.json();
            // this means that whatever C++ returns, it needs to be written in public
            hiddenInTheLeaf(data.currFilePath); // replace with file you want to use in the future
            generateSideBad(data.data);
            console.log(data);
        }
        else
        {
            console.error("Server Error: Error with data transfer for encryption, are you sure the server is on?");
            alert("Server Error: Error with data transfer for encryption, are you sure the server is on  or you inputted the right file type?");
        }

    }
    catch(err)
    {
        console.error(err.message);
        alert("Network error occured: are you sure the server.js is on?");
    }

})

document.getElementById('decodeBut').addEventListener('click', async (e)=>{
    e.preventDefault();
    if(decryptFile == null)
    {
        alert("Please upload a file first before attempring to decode!");
        return;
    }
    //rest of logic for accessing server
    console.log("Preparing to send:", decryptFile.name);
    const formData = new FormData();//attempt to package the file for use
    formData.append('mainfile', decryptFile);

    try{
        /*Fetch fetches a express link that is live via node, 
        this method allows us to pass info directly to server.js to run c++
        Uses the POST method which sends Data to backend server in the body of the request
        */
        const response = await fetch(`http://localhost:5500/huffman?typeConv=${"decrypt"}`, {
            method:'POST',
            body: formData
        });
        if(response.ok){
            console.log("Success! File sent for decryption");
            const data =  await response.json();
            // this means that whatever C++ returns, it needs to be written in public
            hiddenInTheLeaf(data.currFilePath); // replace with file you want to use in the future
            generateSideBad(data.data);
            console.log(data);
        }
        else
        {
            console.error("Server Error: Error with data transfer for decryption, are you sure the server is on?");
            alert("Server Error: Error with data transfer for decryption, are you sure the server is on or you inputted the right file type?");
        }

    }
    catch(err)
    {
        console.error(err.message);
        alert("Network error occured: are you sure the server.js is on?");
    }
})


