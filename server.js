const express = require('express');
const { spawn } = require('child_process');
const path = require('path');

const app = express();
const PORT = 3000;

app.use(express.json());
app.use(express.static('public'));

const CPP_PROGRAM = process.platform === 'win32' 
    ? path.join(__dirname, 'cpp', 'hotel_system.exe')
    : path.join(__dirname, 'cpp', 'hotel_system');

let cppProcess = null;

function startCppProcess() {
    if (cppProcess) {
        cppProcess.kill();
    }
    cppProcess = spawn(CPP_PROGRAM);
    
    cppProcess.stderr.on('data', (data) => {
        console.error(`C++ Error: ${data}`);
    });
    
    cppProcess.on('close', (code) => {
        console.log(`C++ process exited with code ${code}`);
        cppProcess = null;
    });
    
    console.log('✅ C++ Hotel System started');
}

app.post('/execute', (req, res) => {
    const { command } = req.body;
    
    if (!cppProcess) {
        startCppProcess();
    }
    
    let output = '';
    let dataReceived = false;
    
    const timeout = setTimeout(() => {
        if (!dataReceived) {
            res.json({ output: 'ERROR|Timeout' });
        }
    }, 5000);
    
    const onData = (data) => {
        output += data.toString();
        
        if (output.includes('\n')) {
            dataReceived = true;
            clearTimeout(timeout);
            cppProcess.stdout.removeListener('data', onData);
            res.json({ output: output.trim() });
        }
    };
    
    cppProcess.stdout.on('data', onData);
    cppProcess.stdin.write(command + '\n');
});

app.listen(PORT, () => {
    console.log(`🏨 Hotel Booking Server running at http://localhost:${PORT}`);
    startCppProcess();
});

process.on('exit', () => {
    if (cppProcess) {
        cppProcess.kill();
    }
});



















