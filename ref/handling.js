document.addEventListener('DOMContentLoaded', function() {
    const inputArea = document.getElementById('roundedInput');
    const outputArea = document.getElementById('outputArea');
    const optionBtns = document.querySelectorAll('.option-btn');

    let userSelections = []; // Array to store chosen options
    let categoriesChosen = {}; // Track which categories have been chosen

    optionBtns.forEach(btn => {
        btn.addEventListener('click', function() {
            const set = parseInt(this.dataset.set);
            const option = this.dataset.option;

            // Check if this category can be chosen
            if (set === 1 || (set === 2 && categoriesChosen[1]) || (set === 3 && categoriesChosen[1] && categoriesChosen[2])) {
                // Update selected style inside this category
                const sameCategoryBtns = document.querySelectorAll(`[data-set="${set}"]`);
                sameCategoryBtns.forEach(categoryBtn => categoryBtn.classList.remove('selected'));
                this.classList.add('selected');

                // Store or update the selection in the array
                const existingIndex = userSelections.findIndex(selection => selection.category === set);
                if (existingIndex >= 0) {
                    userSelections[existingIndex].option = option;
                } else {
                    userSelections.push({
                        category: set,
                        option: option
                    });
                }

                // Mark this category as chosen
                categoriesChosen[set] = true;

                // Enable next set if available
                if (set < 3) {
                    const nextSetBtns = document.querySelectorAll(`[data-set="${set + 1}"]`);
                    nextSetBtns.forEach(nextBtn => nextBtn.disabled = false);
                } else {
                    // All sets chosen, enable input
                    inputArea.disabled = false;
                    inputArea.focus();
                }

                // Log the selection to output
                outputArea.value += (outputArea.value ? '\n' : '') + `Category ${set}: Option ${option}`;
                outputArea.scrollTop = outputArea.scrollHeight;

                // Log the selections array to console for debugging
                console.log('User Selections:', userSelections);
            }
        });
    });

    // Input handler - clear output on Enter
    inputArea.addEventListener('keydown', function(event) {
        if (event.key === 'Enter' && !event.shiftKey) {
            event.preventDefault();
            const text = inputArea.value.trim();
            if (text) {
                outputArea.value = text; // Clear and set to current input
                inputArea.value = '';
                outputArea.scrollTop = outputArea.scrollHeight;
            }
        }
    });

    // Enter button handler
    document.getElementById('enterBtn').addEventListener('click', function() {
        const text = inputArea.value.trim();
        if (text) {
            outputArea.value = text; // Clear and set to current input
            inputArea.value = '';
            outputArea.scrollTop = outputArea.scrollHeight;
        }
    });

    // Clear button handler
    document.getElementById('clearBtn').addEventListener('click', function() {
        outputArea.value = '';
        inputArea.value = '';
        inputArea.focus();
    });

    // Theme toggle handler
    const modeToggleBtn = document.getElementById('modeToggleBtn');
    modeToggleBtn.addEventListener('click', function() {
        document.body.classList.toggle('light-mode');
        const isLight = document.body.classList.contains('light-mode');
        modeToggleBtn.textContent = isLight ? 'Switch to Night Mode' : 'Switch to Light Mode';
    });
});