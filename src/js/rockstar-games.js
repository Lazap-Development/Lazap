const si = require('systeminformation');

si.osInfo()
  .then(data => console.log(data.platform))
  .catch(error => console.error(error));