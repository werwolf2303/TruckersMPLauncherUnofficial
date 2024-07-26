const babel = require('@babel/core');
const fs = require('fs');
const path = require('path');


const inputFilePath = path.resolve(process.argv[2]);
const outputFilePath = path.resolve(process.argv[3]);
const inputCode = fs.readFileSync(inputFilePath, 'utf8');
const { code } = babel.transformSync(inputCode, {
    filename: inputFilePath,
    presets: ['@babel/preset-react'],
    plugins: [path.resolve(__dirname, './deps/EmbeddedImports/index')]
});
fs.writeFileSync(outputFilePath, code, 'utf8');
console.log("Transformed " + path.basename(inputFilePath) + " to " + path.basename(outputFilePath));
