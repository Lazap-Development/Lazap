const getLauncherIcon = (LauncherName => {
  let icon;
  switch (LauncherName) {
      case "Minecraft":
          icon = "https://cdn.iconscout.com/icon/free/png-256/minecraft-2752120-2284937.png";
          break;
      case "EpicGames":
          icon = "https://progameguides.com/wp-content/uploads/2022/06/Featured-EpicGames-Socket-Open-Error-on-Epic-Games-How-to-fix-900x506.jpg";
          break;
      case "Lutris":
          icon = "https://cdn2.steamgriddb.com/file/sgdb-cdn/logo_thumb/534b36b22aab256cdb15c38b66ea1a74.png";
          break;
      case "Lunar":
          icon = "https://static.planetminecraft.com/files/image/minecraft/texture-pack/2022/051/15323163-actmsw-x_l.webp";
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