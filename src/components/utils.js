async function getBannerResponse(force, id) {
	if (id !== 'allGames') return;
	const gamesList = document.querySelectorAll('#allGamesList > div');

	setTimeout(() => {
		document.getElementById('game-loading-overlay').style.opacity = '0';
		document.getElementById('game-loading-overlay').style.visibility = 'hidden';
	}, 100);
	setTimeout(() =>
		// eslint-disable-next-line max-nested-callbacks
		gamesList.forEach((gameElement) => gameElement.firstElementChild.style.opacity = '1'), 200,
	);
}


export {
	getBannerResponse,
}
