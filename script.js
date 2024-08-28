// server.js
const express = require('express');
const path = require('path');
const fs = require('fs');
const bodyParser = require('body-parser');
const { exec } = require('child_process');

const app = express();
const port = 3000;

// Middleware to parse JSON and urlencoded request bodies
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Serve static files from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

// Route to serve index.html
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'index.html'));
});

// Route to handle POST request from index.html
app.post('/saveToFile', (req, res) => {
  const content = req.body.content; // Assuming 'content' is sent via POST request
  const filePath = path.join(__dirname, 'code.tl');

  // Append content to input.txt file
  fs.writeFile(filePath, content + '\n', (err) => {
    if (err) {
      console.error(err);
      res.status(500).send('Error writing to file');
    } else {
      console.log('Content appended to input.txt');
      res.send('Content appended to input.txt');
    }
  });
});


app.get('/runcpp', (req, res) => {
    const cppFilePath = path.join(__dirname, 'main.cpp'); // Path to your C++ file
    const exeFilePath = path.join(__dirname, 'main'); // Compiled executable path
    
    // Compile the C++ program (assuming it's already compiled)
    exec(`g++ ${cppFilePath} -o ${exeFilePath}`, (error, stdout, stderr) => {
      if (error) {
        console.error('Compilation error:', error.message);
        res.status(500).send('Compilation error');
        return;
      }
      if (stderr) {
        console.error('Compilation stderr:', stderr);
        res.status(500).send('Compilation error');
        return;
      }
  
      // Execute the compiled C++ program
      exec(`${exeFilePath}`, (error, stdout, stderr) => {
        if (error) {
          console.error('Execution error:', error.message);
          res.status(500).send('Execution error');
          return;
        }
        if (stderr) {
          console.error('Execution stderr:', stderr);
          res.status(500).send('Execution error');
          return;
        }
        console.log('C++ program executed successfully');
        res.send(stdout);
      });
    });
});


app.get('/getdata', (req, res) => {
    const filePath = path.join(__dirname, 'Parsed_AST.txt');
  
    // Asynchronously read the contents of the file
    fs.readFile(filePath, 'utf8', (err, data) => {
      if (err) {
        console.error('Error reading file:', err);
        res.status(500).send('Error reading file');
        return;
      }
      // Send the data as JSON to the client
      res.json({ data });
    });
  });



// Start the server
app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});
