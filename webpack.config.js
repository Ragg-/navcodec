const path = require('path');
const webpack = require('webpack');

module.exports = {
    watch: true,
    cache: true,

    context: path.join(__dirname, 'lib_src'),
    target: 'node',
    entry: {
        navcodec: './navcodec'
    },
    resolve: {
        extensions: ['', '.js'],
    },
    externals: /^(?!\.\/|\.\.\/)|\.node$/,
    output: {
        path: path.join(__dirname, 'lib'),
        filename: '[name].js',
        libraryTarget: 'commonjs2',
    },
    module: {
        loaders: [
            {test: /\.js$/, loader: 'babel-loader', exclude: /node_modules/,
             query: {
                presets: [
                    'latest',
                    'stage-3',
                    'stage-2',
                ],
                plugins: [
                    'add-module-exports',
                ]
             }},
        ],
    },
};
