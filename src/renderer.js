/**
 * This file will automatically be loaded by webpack and run in the "renderer" context.
 * To learn more about the differences between the "main" and the "renderer" context in
 * Electron, visit:
 *
 * https://electronjs.org/docs/tutorial/process-model
 *
 * By default, Node.js integration in this file is disabled. When enabling Node.js integration
 * in a renderer process, please be aware of potential security implications. You can read
 * more about security risks here:
 *
 * https://electronjs.org/docs/tutorial/security
 *
 * To enable Node.js integration in this file, open up `main.js` and enable the `nodeIntegration`
 * flag:
 *
 * ```
 *  // Create the browser window.
 *  mainWindow = new BrowserWindow({
 *    width: 800,
 *    height: 600,
 *    webPreferences: {
 *      nodeIntegration: true
 *    }
 *  });
 * ```
 */

import './index.css';

document.addEventListener('DOMContentLoaded', () => {
  const inputArea = document.getElementById('roundedInput');
  const outputArea = document.getElementById('outputArea');
  const optionBtns = document.querySelectorAll('.option-btn');
  const enterBtn = document.getElementById('enterBtn');
  const clearBtn = document.getElementById('clearBtn');
  const modeToggleBtn = document.getElementById('modeToggleBtn');

  let userSelections = [];
  let categoriesChosen = {};

  optionBtns.forEach(btn => {
    btn.addEventListener('click', function () {
      const set = parseInt(this.dataset.set, 10);
      const option = this.dataset.option;

      if (
        set === 1 ||
        (set === 2 && categoriesChosen[1]) ||
        (set === 3 && categoriesChosen[1] && categoriesChosen[2])
      ) {
        const sameCategoryBtns = document.querySelectorAll(`[data-set="${set}"]`);
        sameCategoryBtns.forEach(categoryBtn => categoryBtn.classList.remove('active'));
        this.classList.add('active');

        const existingIndex = userSelections.findIndex(selection => selection.category === set);
        if (existingIndex >= 0) {
          userSelections[existingIndex].option = option;
        } else {
          userSelections.push({ category: set, option });
        }

        categoriesChosen[set] = true;

        if (set < 3) {
          const nextSetBtns = document.querySelectorAll(`[data-set="${set + 1}"]`);
          nextSetBtns.forEach(nextBtn => {
            nextBtn.disabled = false;
          });
        } else {
          inputArea.disabled = false;
          inputArea.focus();
        }

        outputArea.value += (outputArea.value ? '\n' : '') + `Category ${set}: Option ${option}`;
        outputArea.scrollTop = outputArea.scrollHeight;
        console.log('User Selections:', userSelections);
      }
    });
  });

  const processInput = () => {
    const text = inputArea.value.trim();
    if (text) {
      outputArea.value = text;
      inputArea.value = '';
      outputArea.scrollTop = outputArea.scrollHeight;
    }
  };

  inputArea.addEventListener('keydown', event => {
    if (event.key === 'Enter' && !event.shiftKey) {
      event.preventDefault();
      processInput();
    }
  });

  enterBtn.addEventListener('click', processInput);

  clearBtn.addEventListener('click', () => {
    outputArea.value = '';
    inputArea.value = '';
    inputArea.focus();
  });

  modeToggleBtn.addEventListener('click', () => {
    document.body.classList.toggle('light-mode');
    const isLight = document.body.classList.contains('light-mode');
    modeToggleBtn.textContent = isLight ? 'Switch to Night Mode' : 'Switch to Light Mode';
  });
});

// class inputOptions {
//   constructor(processMode, functionMode, additionalOption) {
//     this.processMode = processMode;
//     this.functionMode = functionMode[0];
//     this.functionModeValue = functionMode[1];
//     this.additionalOption = additionalOption[0];
//     this.additionalOptionValue = additionalOption[1];
//   }

//   get processMode() {
//     return this.processMode;
//   }

//   set processMode(value) {
//     this.processMode = value;
//   }

//   get functionMode() {
//     return this.functionMode;
//   }

//   set functionMode(value) {
//     this.functionMode = value;
//   }

//   get functionModeValue() {
//     return this.functionModeValue;
//   }

//   set functionModeValue(value) {
//     this.functionModeValue = value;
//   }

//   get additionalOption() {
//     return this.additionalOption;
//   }

//   set additionalOption(value) {
//     this.additionalOption = value;
//   }

//   get additionalOptionValue() {
//     return this.additionalOptionValue;
//   }

//   set additionalOptionValue(value) {
//     this.additionalOptionValue = value;
//   }
// };