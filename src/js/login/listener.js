const signUpButton = document.getElementById('signUp');
const signInButton = document.getElementById('signIn');
const container = document.getElementById('container');
const alertbox = document.querySelector('.alert-box');
const alertboxcross = document.getElementById('alertboxexit');

alertboxcross.addEventListener('click', function() {
	alertbox.style.display = 'none';
});
signUpButton.addEventListener('click', () => {
	container.classList.add('right-panel-active');
});

signInButton.addEventListener('click', () => {
	container.classList.remove('right-panel-active');
});
