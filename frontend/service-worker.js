const CACHE_NAME = 'revisepdf-v1';
const ASSETS = [
  '/',
  '/app',
  '/features',
  '/index.html',
  '/home.html',
  '/features.html',
  '/app.js',
  '/manifest.json',
  '/service-worker.js',
  '/robots.txt',
  '/sitemap.xml'
];
self.addEventListener('install', (event) => {
  event.waitUntil(
    caches.open(CACHE_NAME).then((cache) => {
      return Promise.all(
        ASSETS.map(asset => {
          return fetch(asset).then(response => {
            if (response.ok) {
              return cache.put(asset, response);
            }
          }).catch(() => {});
        })
      );
    })
  );
});
self.addEventListener('fetch', (event) => {
  if (event.request.method !== 'GET') {
    return;
  }
  event.respondWith(
    caches.match(event.request).then((response) => {
      if (response) {
        return response;
      }
      return fetch(event.request).then((response) => {
        if (!response || response.status !== 200 || response.type !== 'basic') {
          return response;
        }
        const responseToCache = response.clone();
        caches.open(CACHE_NAME).then((cache) => {
          cache.put(event.request, responseToCache);
        });
        return response;
      });
    }).catch(() => {
      return caches.match('/app');
    })
  );
});

