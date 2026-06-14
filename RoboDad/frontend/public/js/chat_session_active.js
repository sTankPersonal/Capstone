var path = window.location.pathname;
document.querySelectorAll('.chat-list-item').forEach(function (a) {
    if (a.getAttribute('href') === path) a.classList.add('active');
});
