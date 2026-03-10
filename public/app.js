//variables
const encryptDropZone = document.getElementById('Encryption');
const decryptDropZone = document.getElementById('Decryption');
const encryptInp = document.getElementById('eFileDrop');
const decryptInp = document.getElementById('dFileDrop');
let encryptFile=null;
let decryptFile = null;


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
    if(e.target.files.length>0)
    {
        handleEFile(e.target.files[0]);
        //whole process ensures the element gets updated since
        //html input for files is finnicky with drag and drop method
        const dataTransfer = new DataTransfer();
        dataTransfer.items.add(encryptFile);
        encryptInp.files= dataTransfer.files;
    }
});

encryptDropZone.addEventListener('click', ()=>{
    encryptInp.click();
});

encryptInp.addEventListener('change',(e)=>{
    if(e.target.files.length>0)
    {
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
    if(e.target.files.length>0)
    {
        handleDFile(e.target.files[0]);
        const dataTransfer = new DataTransfer();
        dataTransfer.items.add(decryptFile);
        decryptInp.files= dataTransfer.files;
    }
});



decryptDropZone.addEventListener('click', ()=>{
    decryptInp.click();
});

decryptInp.addEventListener('change',(e)=>{
    if(e.target.files.length>0)
    {
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
        }
        else
        {
            console.error("Server Error: Error with data transfer for encryption, are you sure the server is on?");
            alert("Server Error: Error with data transfer for encryption, are you sure the server is on?");
        }

    }
    catch(err)
    {
        console.error("Network error occured: are you sure the server.js is on?", err);
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
        }
        else
        {
            console.error("Server Error: Error with data transfer for decryption, are you sure the server is on?");
            alert("Server Error: Error with data transfer for decryption, are you sure the server is on?");
        }

    }
    catch(err)
    {
        console.error("Network error occured: are you sure the server.js is on?", err);
        alert("Network error occured: are you sure the server.js is on?");
    }
})



