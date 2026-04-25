module.exports = {
  /**
   * This is the main entry point for your application, it's the first file
   * that runs in the main process.
   */
  externals: {
    // Replace 'elementProcess' with the name you use in require()
    'C:/Users/ADMIN/Documents/Teacher_tools/V4/teaching/build/Release/elementProcess.node': 'commonjs C:/Users/ADMIN/Documents/Teacher_tools/V4/teaching/build/Release/elementProcess.node'
  },
  entry: './src/main.js',
  // Put your normal webpack config below here
  module: {
   rules: [
      {
        test: /\.node$/,
        loader: "node-loader", // Requires: npm install --save-dev node-loader
      },
      {
        test: /\.css$/,
        use: [{ loader: 'style-loader' }, { loader: 'css-loader' }],
      },
      {
        test: /\.(m?js|node)$/,
        parser: { amd: false },
      },
    ],
  },
};
