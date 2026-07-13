(function () {
    var form = document.getElementById('resend-form');
    var emailInput = document.getElementById('email');
    var statusBox = document.getElementById('status-message');
    var errorBox = document.getElementById('form-error');
    var subtitle = document.getElementById('check-email-subtitle');

    var params = new URLSearchParams(window.location.search);
    var email = params.get('email') || '';
    if (email) {
        emailInput.value = email;
    }

    function showStatus(message) {
        statusBox.textContent = message;
        statusBox.hidden = false;
    }

    function showError(message) {
        errorBox.textContent = message;
        errorBox.hidden = false;
    }

    if (params.get('sent') === '1') {
        showStatus('A new verification email has been sent.');
    } else if (params.get('expired') === '1') {
        subtitle.textContent = 'That verification link expired or is no longer valid. Enter your email below to get a new one.';
    }

    form.addEventListener('submit', function (e) {
        e.preventDefault();

        var value = emailInput.value.trim();
        if (!value) return;

        var submitBtn = form.querySelector('button[type="submit"]');
        submitBtn.disabled = true;

        var body = new URLSearchParams({ email: value });
        fetch('/auth/resend-verification', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: body.toString()
        }).then(function (res) {
            if (res.redirected) {
                window.location.href = res.url;
                return;
            }
            submitBtn.disabled = false;
        }).catch(function () {
            submitBtn.disabled = false;
            showError('Unable to reach the server. Please try again.');
        });
    });
})();
