import http from 'http';

const options = {
  hostname: 'localhost',
  port: 3001,
  path: '/api/books',
  method: 'POST',
  headers: {
    'Content-Type': 'application/json',
    'Content-Length': 150
  }
};

const req = http.request(options, (res) => {
  console.log('状态码:', res.statusCode);
  console.log('响应头:', res.headers);
  
  res.on('data', (chunk) => {
    console.log('响应体:', chunk.toString());
  });
  
  res.on('end', () => {
    console.log('请求完成');
  });
});

req.on('error', (e) => {
  console.error('请求错误:', e.message);
});

req.write(JSON.stringify({
  title: 'Test Book',
  author: 'Test Author', 
  isbn: '9781234567899',
  price: 29.99,
  category: 'Test',
  description: 'Test',
  published_date: '2024-01-01'
}));
req.end();