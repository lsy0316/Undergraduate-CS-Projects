import mysql from 'mysql2/promise';

async function testConnection() {
  console.log('测试 MySQL 连接...');
  
  const ports = [3306, 3307, 3308];
  
  for (const port of ports) {
    console.log(`\n尝试端口 ${port}...`);
    try {
      const pool = mysql.createPool({
        host: 'localhost',
        user: 'root',
        password: 'test1',
        database: 'test1',
        port: port,
        connectTimeout: 3000
      });
      
      const connection = await pool.getConnection();
      const [rows] = await connection.execute('SELECT 1 as test');
      console.log(`✅ 端口 ${port} 连接成功!`);
      console.log('查询结果:', rows);
      connection.release();
      
      try {
        const [books] = await pool.execute('SELECT * FROM books LIMIT 5');
        console.log('books 表数据:', books);
      } catch (e) {
        console.log('books 表查询失败:', e.message);
      }
      
      return;
    } catch (error) {
      console.log(`❌ 端口 ${port} 连接失败:`, error.message);
    }
  }
  
  console.log('\n所有端口都无法连接!');
}

testConnection();