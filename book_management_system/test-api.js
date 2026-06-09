import http from 'http';

function httpGet(url) {
  return new Promise((resolve, reject) => {
    http.get(url, (res) => {
      let data = '';
      res.on('data', (chunk) => { data += chunk; });
      res.on('end', () => resolve({ status: res.statusCode, body: data }));
    }).on('error', reject);
  });
}

function httpPost(url, body) {
  return new Promise((resolve, reject) => {
    const data = JSON.stringify(body);
    const options = {
      method: 'POST',
      headers: { 'Content-Type': 'application/json', 'Content-Length': data.length }
    };
    
    const req = http.request(url, options, (res) => {
      let responseData = '';
      res.on('data', (chunk) => { responseData += chunk; });
      res.on('end', () => {
        resolve({ status: res.statusCode, body: responseData });
      });
    });
    
    req.on('error', reject);
    req.write(data);
    req.end();
  });
}

async function testAPI() {
  console.log('测试图书管理系统 API...');
  
  console.log('\n1. 测试 GET /api/books');
  try {
    const result = await httpGet('http://localhost:3001/api/books');
    console.log('状态码:', result.status);
    if (result.status === 200) {
      const data = JSON.parse(result.body);
      console.log('✅ 获取成功!');
      console.log('图书数量:', data.length);
    } else {
      console.log('❌ 获取失败:', result.body);
    }
  } catch (error) {
    console.log('❌ 获取失败:', error.message);
  }
  
  console.log('\n2. 测试 POST /api/books');
  try {
    const newBook = {
      title: '测试图书',
      author: '测试作者',
      isbn: '9781234567891',
      price: 29.99,
      category: '测试',
      description: '这是一本测试图书',
      published_date: '2024-01-01'
    };
    
    console.log('发送的数据:', JSON.stringify(newBook));
    
    const result = await httpPost('http://localhost:3001/api/books', newBook);
    console.log('状态码:', result.status);
    console.log('响应体:', result.body);
    if (result.status === 201) {
      const data = JSON.parse(result.body);
      console.log('✅ 添加成功!');
      console.log('新图书ID:', data.id);
    } else {
      console.log('❌ 添加失败');
    }
  } catch (error) {
    console.log('❌ 添加失败:', error.message);
  }
}

testAPI();