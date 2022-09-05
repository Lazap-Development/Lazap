const getLauncherIcon = (game => {
    let icon;
    switch (game.LauncherName) {
        case "Minecraft":
            icon = "https://cdn.iconscout.com/icon/free/png-256/minecraft-2752120-2284937.png";
            break;
        case "EpicGames":
            icon = "https://progameguides.com/wp-content/uploads/2022/06/Featured-EpicGames-Socket-Open-Error-on-Epic-Games-How-to-fix-900x506.jpg";
            break;
        case "Lutris":
            icon = "https://www.gamingonlinux.com/uploads/tagline_gallery/lutris.jpg";
            break;
        case "Lunar":
            icon = "https://pbs.twimg.com/profile_images/1479900487814205448/SLinOoFW_400x400.jpg";
            break;
        case "RiotGames":
            icon = "https://static.wikia.nocookie.net/leagueoflegends/images/5/53/Riot_Games_logo_icon.png/revision/latest?cb=20220302144707";
            break;
        case "RockstarGames":
            icon = "https://cdn.icon-icons.com/icons2/2407/PNG/512/rockstar_games_icon_146104.png";
            break;
        case "Steam":
            icon = "https://upload.wikimedia.org/wikipedia/commons/thumb/8/83/Steam_icon_logo.svg/512px-Steam_icon_logo.svg.png";
            break;
        case "Uplay":
            icon = "https://play-lh.googleusercontent.com/f868E2XQBpfl677hykMnZ4_HlKqrOs0fUhuwy0TC9ZI_PQLn99RtBV2kQ7Z50OtQkw=w240-h480";
            break;
        case "XboxGames":
            icon = "https://www.freeiconspng.com/uploads/xbox-icon-8.jpg";
        break;
        default:
            icon = "https://icon-library.com/images/file-icon-size/file-icon-size-19.jpg";
            break;
    }
    return icon;
})

module.exports = {
    getLauncherIcon
}