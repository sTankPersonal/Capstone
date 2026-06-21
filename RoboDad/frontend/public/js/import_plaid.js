(function () {
    document.getElementById('connect-btn').addEventListener('click', async function () {
        var res  = await fetch('/user/plaid/link-token');
        var data = await res.json();

        if (!res.ok) {
            alert('Could not start bank connection:\n' + (data.error || res.status));
            return;
        }

        Plaid.create({
            token: data.link_token,
            onSuccess: function (publicToken) {
                document.getElementById('public_token').value = publicToken;
                document.getElementById('plaid-form').submit();
            },
            onExit: function () {}
        }).open();
    });
})();
