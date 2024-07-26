const fs = require('fs');
const ppath = require('path');
const babel = require('@babel/core');
const parser = require("@babel/parser")

module.exports = function ({ types: t }) {
    return {
        visitor: {
            ImportDeclaration(path, state) {
                const importPath = path.node.source.value;
                const filePath = ppath.resolve(state.file.opts.filename, '..', importPath + ".jsx");
                const fileContent = fs.readFileSync(filePath, 'utf8');
                const asyncTransformResult = babel.transformSync(fileContent, {
                    presets: ['@babel/preset-react']
                });
                var rawCode = asyncTransformResult.code;
                rawCode = rawCode.replaceAll("export default ", "");
                rawCode = rawCode.replaceAll("export ", "");
                path.replaceWith(parser.parse(rawCode));
            }
        }
    };
};