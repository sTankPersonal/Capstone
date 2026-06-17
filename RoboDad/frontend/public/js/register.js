(function () {
    var form = document.getElementById('register-form');
    var errorBox = document.getElementById('form-error');
    var passwordInput = document.getElementById('password');
    var confirmInput = document.getElementById('confirm_password');
    var requirementItems = document.querySelectorAll('#password-requirements li');

    var rules = {
        length: function (pw) { return pw.length >= 8; },
        uppercase: function (pw) { return /[A-Z]/.test(pw); },
        lowercase: function (pw) { return /[a-z]/.test(pw); },
        number: function (pw) { return /[0-9]/.test(pw); },
        special: function (pw) { return /[^A-Za-z0-9]/.test(pw); },
        match: function (pw, confirm) { return pw.length > 0 && pw === confirm; }
    };

    function showError(message) {
        errorBox.textContent = message;
        errorBox.hidden = false;
    }

    function clearError() {
        errorBox.textContent = '';
        errorBox.hidden = true;
    }

    // Returns true when every password rule is satisfied.
    function evaluateRequirements() {
        var pw = passwordInput.value;
        var confirm = confirmInput.value;
        var allMet = true;

        requirementItems.forEach(function (item) {
            var rule = item.dataset.rule;
            var met = rule === 'match' ? rules.match(pw, confirm) : rules[rule](pw);
            // Hide each requirement once it is met; show it again if it stops being met.
            item.classList.toggle('met', met);
            if (!met) {
                allMet = false;
            }
        });

        return allMet;
    }

    passwordInput.addEventListener('input', function () {
        evaluateRequirements();
        clearError();
    });
    confirmInput.addEventListener('input', function () {
        evaluateRequirements();
        clearError();
    });

    form.addEventListener('submit', function (e) {
        e.preventDefault();
        clearError();

        var firstName = document.getElementById('first_name').value.trim();
        var lastName = document.getElementById('last_name').value.trim();
        var email = document.getElementById('email').value.trim();
        var password = passwordInput.value;

        if (!firstName || !lastName || !email) {
            showError('Please fill in all fields.');
            return;
        }

        if (!evaluateRequirements()) {
            showError('Please meet all of the password requirements before continuing.');
            return;
        }

        var submitBtn = form.querySelector('.btn-signup');
        submitBtn.disabled = true;

        var params = new URLSearchParams({
            first_name: firstName,
            last_name: lastName,
            email: email,
            password: password
        });
        fetch('/auth/register', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: params.toString()
        }).then(function (res) {
            if (res.redirected) {
                window.location.href = res.url;
                return;
            }
            submitBtn.disabled = false;
            if (res.status === 400) {
                showError('We could not create your account. This email may already be registered.');
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
