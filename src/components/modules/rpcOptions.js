let currentRpc = "home";

const rpcOptions = {
  home: {
    details: "Home",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "home",
  },
  recent: {
    details: "Recent Games",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "recent",
  },
  games: {
    details: "All Games",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "games",
  },
  favourites: {
    details: "Favourite Games",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "favourites",
  },
  messages: {
    details: "Messages",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "messages",
  },
  activity: {
    details: "Activities",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "activity",
  },
  friends: {
    details: "All Friends",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "friends",
  },
};

const selectOption = (option) => {
  const { home, recent, games, favourites, messages, activity, friends } =
    rpcOptions;
  switch (option) {
    case "home":
      currentRpc = "home";
      return home;
    case "recent":
      currentRpc = "recent";
      return recent;
    case "games":
      currentRpc = "games";
      return games;
    case "favourites":
      currentRpc = "favourites";
      return favourites;
    case "messages":
      currentRpc = "messages";
      return messages;
    case "activity":
      currentRpc = "activity";
      return activity;
    case "friends":
      currentRpc = "friends";
      return friends;
    default:
      throw Error("Invalid Option!");
  }
};

export { selectOption, currentRpc };
