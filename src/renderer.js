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
  let additionalValue = 0;
  let inputAddValue = false;

  optionBtns.forEach(btn => {
  btn.addEventListener('click', function () {
    const set = parseInt(this.dataset.set, 10);
    const option = this.textContent.trim();

    // Validation check for sequence
    if (
      set === 1 ||
      (set === 2 && categoriesChosen[1]) ||
      (set === 3 && categoriesChosen[1] && categoriesChosen[2])
    ) {
      // 1. UI: Set active class
      const sameCategoryBtns = document.querySelectorAll(`[data-set="${set}"]`);
      sameCategoryBtns.forEach(categoryBtn => categoryBtn.classList.remove('active'));
      this.classList.add('active');

      // 2. Update state
      const existingIndex = userSelections.findIndex(selection => selection.hasOwnProperty(set));
      if (existingIndex >= 0) {
        userSelections[existingIndex][set] = option;
      } else {
        let newSelection = {};
        newSelection[set] = option;
        userSelections.push(newSelection);
      }
      categoriesChosen[set] = true;

      // 3. GET CURRENT SELECTIONS FOR LOGIC
      const getVal = (s) => (userSelections.find(sel => sel.hasOwnProperty(s)) || {})[s];
      const s1 = getVal(1);
      const s2 = getVal(2);

      // 4. FILTER SET 3 BUTTONS (Run this every time Set 1 or Set 2 changes)
      const set3Btns = document.querySelectorAll('[data-set="3"]');
      if (s1 === "Sentence") {
        set3Btns.forEach(b => {
          if (b.textContent.includes("first letter or word") || b.textContent.includes("index")) {
            b.style.display = 'none';
            b.classList.remove('active'); // Clean up if it was active
          } else {
            b.style.display = 'block';
          }
        });
      } else {
        set3Btns.forEach(b => b.style.display = 'block');
      }

      // 5. HANDLE FLOW TRANSITIONS
      if (set === 2 && option === "Scramble") {
        // Scramble logic
        inputArea.disabled = false;
        inputArea.placeholder = "Enter text here";
        inputAddValue = false;
        inputArea.focus();
        document.getElementById('set3').style.opacity = '0.3';
      } 
      else if (set < 3) {
        // Unlock next set
        document.getElementById('set3').style.opacity = '1';
        const nextSetBtns = document.querySelectorAll(`[data-set="${set + 1}"]`);
        nextSetBtns.forEach(nextBtn => nextBtn.disabled = false);
      } 
      else if (set === 3) {
        // Final input preparation
        inputArea.disabled = false;
        inputAddValue = true;
        if (option.includes("percentage") || option.includes("index")) {
          inputArea.placeholder = "Enter a number";
        } else if (option.includes("word or character")) {
          inputArea.placeholder = "Enter a word/character";
        } else if (option.includes("first letter or word")) {
          inputAddValue = false;
        }
        inputArea.focus();
      }

      outputArea.value += (outputArea.value ? '\n' : '') + `Category ${set}: ${option}`;
      outputArea.scrollTop = outputArea.scrollHeight;
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
        additionalValue = String(text);
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