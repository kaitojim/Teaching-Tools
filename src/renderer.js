require('./index.css');

document.addEventListener('DOMContentLoaded', () => {
  const inputArea = document.getElementById('roundedInput');
  const outputArea = document.getElementById('outputArea');
  const optionBtns = document.querySelectorAll('.option-btn');
  const enterBtn = document.getElementById('enterBtn');
  const clearBtn = document.getElementById('clearBtn');
  const modeToggleBtn = document.getElementById('modeToggleBtn');

  let userSelections = [];
  let categoriesChosen = {};
  let additionalValue = {};
  let inputAddValue = false;

  optionBtns.forEach(btn => {
    btn.addEventListener('click', function () {
      // const set = parseInt(this.dataset.set, 10);
      //const option = this.dataset.option;
      const set = parseInt(this.dataset.set, 10);
      const option = this.textContent.trim();

      if (
        set === 1 ||
        (set === 2 && categoriesChosen[1]) ||
        (set === 3 && categoriesChosen[1] && categoriesChosen[2])
      ) {
        const sameCategoryBtns = document.querySelectorAll(`[data-set="${set}"]`);
        sameCategoryBtns.forEach(categoryBtn => categoryBtn.classList.remove('active'));
        this.classList.add('active');

        // const existingIndex = userSelections.findIndex(selection => selection.category === set);
        const existingIndex = userSelections.findIndex(selection => selection.hasOwnProperty(set));
        if (existingIndex >= 0) {
          // userSelections[existingIndex].option = option;
          userSelections[existingIndex][set] = option;
        } else {
          // userSelections.push({ category: set, option });
          let newSelection = {};
          newSelection[set] = option;
          userSelections.push(newSelection);
        }

        categoriesChosen[set] = true;

        if (set < 3) {
          const nextSetBtns = document.querySelectorAll(`[data-set="${set + 1}"]`);
          nextSetBtns.forEach(nextBtn => {
            nextBtn.disabled = false;
          });
        } else {
          inputAddValue = true;
          inputArea.disabled = false;
          if (option.includes("Fill in the blank (percentage)") || option.includes("Fill in the blank (index)")) {
            inputArea.placeholder = "Enter a number";
          } 
          else if(option.includes("Fill in the blank (word or letter)")) {
            inputArea.placeholder = "Enter a word/character";
          }
          else if(option.includes("Fill in the blank (first letter or word)")) {inputAddValue = false;}
          
          inputArea.focus();
        }

        outputArea.value += (outputArea.value ? '\n' : '') + `Category ${set}: Option ${option}`;
        outputArea.scrollTop = outputArea.scrollHeight;
        console.log('User Selections:', userSelections);
      }
    });
  });

  const processInput = async () => { // Added async
    const text = inputArea.value.trim();
    if(!text)
    {
      return;
    }
    else if (inputAddValue) 
    {
      const currentMode = userSelections.find(s => s[3]); // Find Set 3 selection
      const modeName = currentMode ? currentMode[3] : "";

      if (modeName.includes("percentage") || modeName.includes("index"))
      {
        additionalValue = Number(text);
        if (isNaN(additionalValue))
        {
          alert("Please enter a valid number");
          inputArea.value = "";
          return;
        }
      }
      else
      {
        additionalValue = text;
      }
      inputAddValue   = false;
      inputArea.placeholder = "Enter text here";
      inputArea.value = "";
    }
    else 
    {
      try {
        // Call the bridge instead of handleInput directly
        // This sends the data to the Main process and waits for the result
        let array = await window.cppAPI.transform(text, userSelections, additionalValue);
        
        console.log(array);
        
        outputArea.value = ""; // Clear before loop if needed
        for(let i = 0; i < array.length; i++) {
          const index = i + 1;
          outputArea.value += `${index}/ ${array[i]} \n`;
        }
        
        // 3. Reset UI back to normal
        inputArea.placeholder = "Enter text here";
        inputArea.value = '';
        outputArea.scrollTop = outputArea.scrollHeight;
        additionalValue = '';
      } catch (err) {
        console.error("C++ Addon Error:", err);
      }
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