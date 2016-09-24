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
                    'stage-3',
                    'stage-2',
                ],
                plugins: [
                    'transform-es2015-modules-commonjs',
                    'transform-class-properties',
                    'transform-es2015-classes',
                    'add-module-exports',
                ]
             }},
        ],
    },
};
