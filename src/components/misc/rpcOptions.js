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
  favs: {
    details: "Favourite Games",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "favs",
  },
  monitor: {
    details: "Monitoring",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "monitor",
  },
  overclock: {
    details: "Overclocking",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "overclock",
  },
  benchmark: {
    details: "Benchmarking",
    largeImage: "lazap_icon",
    largeText: "Lazap",
    smallText: "Lazap",
    id: "benchmark",
  },
};

const selectOption = (option) => {
  const selectedOption = rpcOptions[option];
  if (selectedOption) {
    currentRpc = option;
    return selectedOption;
  }
  throw new Error("Invalid Option!");
};

export { selectOption, currentRpc };
