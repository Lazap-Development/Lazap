const { defineConfig } = require('@vue/cli-service')
module.exports = defineConfig({
  transpileDependencies: true,
  pages: {
    index: 'src/js/main.js',
    overlay: 'src/js/overlay.js'
  }
})
