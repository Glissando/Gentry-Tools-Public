chrome.app.runtime.onLaunched.addListener(function() {
  chrome.tabs.create({
	'url':'index.html'
  });
});