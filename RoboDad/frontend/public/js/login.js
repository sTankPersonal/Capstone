(function () {
    var form = document.getElementById('login-form');
    var errorBox = document.getElementById('form-error');
    var emailInput = document.getElementById('email');
    var passwordInput = document.getElementById('password');

    function showError(message) {
        errorBox.textContent = message;
        errorBox.hidden = false;
    }

    function clearError() {
        errorBox.textContent = '';
        errorBox.hidden = true;
    }

    // Hide the error as soon as the user starts correcting their input.
    [emailInput, passwordInput].forEach(function (input) {
        input.addEventListener('input', clearError);
    });

    form.addEventListener('submit', function (e) {
        e.preventDefault();
        clearError();

        var email = emailInput.value.trim();
        var password = passwordInput.value;

        if (!email || !password) {
            showError('Please enter both your email and password.');
            return;
        }

        if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email)) {
            showError('Please enter a valid email address.');
            return;
        }

        var submitBtn = form.querySelector('.btn-signin');
        submitBtn.disabled = true;

        var params = new URLSearchParams({ email: email, password: password });
        fetch('/auth/login', {
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
                showError('Incorrect email or password. Please try again.');
            } else if (res.status === 400) {
                showError('Please enter both your email and password.');
            } else {
                showError('Something went wrong. Please try again.');
            }
        }).catch(function () {
            submitBtn.disabled = false;
            showError('Unable to reach the server. Please check your connection and try again.');
        });
    });
})();
