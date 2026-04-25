module.exports = {};
const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('cppAPI', {
  transform: (text, selections, val) => ipcRenderer.invoke('transform-element', text, selections, val)
});