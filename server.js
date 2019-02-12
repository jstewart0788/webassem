//Express

const express = require('express');
const app = express();
const PORT = 2222;

//serve static files from /public
app.use( express.static('public',{
    setHeaders: (res, path) => {
        //Serve .wasm files with correct mime type
        if (path.endsWith('.wasm'))
            res.set('Content-Type', 'application/wasm')
    }
})); 

//start server
app.listen(PORT, () => console.log(`Server Running on port ${PORT}`));