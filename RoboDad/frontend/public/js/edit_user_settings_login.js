(function () {
    var form = document.getElementById('change-password-form');
    var errorBox = document.getElementById('form-error');
    var currentInput = document.getElementById('current_password');
    var newInput = document.getElementById('new_password');
    var confirmInput = document.getElementById('confirm_password');
    var requirementItems = document.querySelectorAll('#password-requirements li');

    // Mirrors the rules enforced on the sign-up page so both forms share one standard.
    var rules = {
        length: function (pw) { return pw.length >= 8; },
        uppercase: function (pw) { return /[A-Z]/.test(pw); },
        lowercase: function (pw) { return /[a-z]/.test(pw); },
        number: function (pw) { return /[0-9]/.test(pw); },
        special: function (pw) { return /[^A-Za-z0-9]/.test(pw); },
        different: function (pw, confirm, current) { return pw.length > 0 && pw !== current; },
        match: function (pw, confirm) { return pw.length > 0 && pw === confirm; }
    };

    function showError(message) {
        errorBox.textContent = message;
        errorBox.style.display = 'block';
    }

    function clearError() {
        errorBox.textContent = '';
        errorBox.style.display = 'none';
    }

    // Returns true when every new-password rule is satisfied.
    function evaluateRequirements() {
        var pw = newInput.value;
        var confirm = confirmInput.value;
        var current = currentInput.value;
        var allMet = true;

        requirementItems.forEach(function (item) {
            var rule = item.dataset.rule;
            var met = rules[rule](pw, confirm, current);
            // Hide each requirement once it is met; show it again if it stops being met.
            item.classList.toggle('met', met);
            if (!met) {
                allMet = false;
            }
        });

        return allMet;
    }

    [currentInput, newInput, confirmInput].forEach(function (input) {
        input.addEventListener('input', function () {
            evaluateRequirements();
            clearError();
        });
    });

    form.addEventListener('submit', function (e) {
        e.preventDefault();
        clearError();

        if (!currentInput.value) {
            showError('Please enter your current password.');
            return;
        }

        if (!evaluateRequirements()) {
            showError('Please meet all of the new password requirements before continuing.');
            return;
        }

        var submitBtn = form.querySelector('.btn-primary');
        submitBtn.disabled = true;

        var params = new URLSearchParams({
            current_password: currentInput.value,
            new_password: newInput.value,
            confirm_password: confirmInput.value
        });
        fetch(form.action, {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: params.toString()
        }).then(function (res) {
            if (res.redirected) {
                window.location.href = res.url;
                return;
            }
            submitBtn.disabled = false;
            if (res.status === 401) {
                showError('Your current password is incorrect.');
            } else if (res.status === 400) {
                return res.text().then(function (text) {
                    showError(text || 'Please check your input and try again.');
                });
            } else {
                showError('Something went wrong. Please try again.');
            }
        }).catch(function () {
            submitBtn.disabled = false;
            showError('Unable to reach the server. Please check your connection and try again.');
        });
    });

    // Show the initial state of the requirements on page load.
    evaluateRequirements();
})();
