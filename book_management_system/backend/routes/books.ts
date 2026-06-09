import express, { type Request, type Response } from 'express';
import pool from '../db';

const router = express.Router();

interface Book {
  id: number;
  title: string;
  author: string;
  isbn: string;
  price: number;
  category: string;
  description: string;
  published_date: string;
}

function transformBook(book: any): Book {
  return {
    ...book,
    price: Number(book.price)
  };
}

router.get('/', async (req: Request, res: Response) => {
  try {
    const { search } = req.query;
    let query = 'SELECT id, title, author, isbn, price, category, description, published_date FROM books';
    let params: string[] = [];

    if (search) {
      query += ' WHERE title LIKE ? OR author LIKE ?';
      params.push(`%${search}%`, `%${search}%`);
    }

    const [rows] = await pool.execute(query, params);
    const books = (rows as any[]).map(transformBook);
    res.json(books);
  } catch (error) {
    console.error('Error fetching books:', error);
    res.status(500).json({ error: 'Failed to fetch books' });
  }
});

router.get('/:id', async (req: Request, res: Response) => {
  try {
    const { id } = req.params;
    const [rows] = await pool.execute('SELECT id, title, author, isbn, price, category, description, published_date FROM books WHERE id = ?', [id]);
    const books = (rows as any[]).map(transformBook);
    
    if (books.length === 0) {
      return res.status(404).json({ error: 'Book not found' });
    }
    
    res.json(books[0]);
  } catch (error) {
    console.error('Error fetching book:', error);
    res.status(500).json({ error: 'Failed to fetch book' });
  }
});

router.post('/', async (req: Request, res: Response) => {
  try {
    console.log('收到 POST 请求');
    console.log('请求体:', req.body);
    console.log('Content-Type:', req.headers['content-type']);
    
    if (!req.body) {
      console.log('错误: 请求体为空');
      return res.status(400).json({ error: '请求体为空' });
    }
    
    const { title, author, isbn, price, category, description, published_date } = req.body;
    
    console.log('解析的字段:', { title, author, isbn, price, category, description, published_date });
    
    if (!title) {
      console.log('错误: 缺少 title');
      return res.status(400).json({ error: '缺少必要字段: title' });
    }
    if (!author) {
      console.log('错误: 缺少 author');
      return res.status(400).json({ error: '缺少必要字段: author' });
    }
    if (!isbn) {
      console.log('错误: 缺少 isbn');
      return res.status(400).json({ error: '缺少必要字段: isbn' });
    }
    if (!price) {
      console.log('错误: 缺少 price');
      return res.status(400).json({ error: '缺少必要字段: price' });
    }
    if (!category) {
      console.log('错误: 缺少 category');
      return res.status(400).json({ error: '缺少必要字段: category' });
    }
    if (!published_date) {
      console.log('错误: 缺少 published_date');
      return res.status(400).json({ error: '缺少必要字段: published_date' });
    }
    
    const [result] = await pool.execute(
      'INSERT INTO books (title, author, isbn, price, category, description, published_date) VALUES (?, ?, ?, ?, ?, ?, ?)',
      [title, author, isbn, price, category, description, published_date]
    );

    const insertResult = result as { insertId: number };
    const [rows] = await pool.execute('SELECT id, title, author, isbn, price, category, description, published_date FROM books WHERE id = ?', [insertResult.insertId]);
    const newBook = transformBook((rows as any[])[0]);
    
    console.log('添加成功:', newBook.id);
    res.status(201).json(newBook);
  } catch (error: any) {
    console.error('Error creating book:', error.message);
    console.error('Error stack:', error.stack);
    res.status(500).json({ error: 'Failed to create book', details: error.message });
  }
});

router.put('/:id', async (req: Request, res: Response) => {
  try {
    const { id } = req.params;
    const { title, author, isbn, price, category, description, published_date } = req.body;
    
    await pool.execute(
      'UPDATE books SET title = ?, author = ?, isbn = ?, price = ?, category = ?, description = ?, published_date = ? WHERE id = ?',
      [title, author, isbn, price, category, description, published_date, id]
    );

    const [rows] = await pool.execute('SELECT id, title, author, isbn, price, category, description, published_date FROM books WHERE id = ?', [id]);
    const updatedBook = transformBook((rows as any[])[0]);
    
    if (!updatedBook) {
      return res.status(404).json({ error: 'Book not found' });
    }
    
    res.json(updatedBook);
  } catch (error) {
    console.error('Error updating book:', error);
    res.status(500).json({ error: 'Failed to update book' });
  }
});

router.delete('/:id', async (req: Request, res: Response) => {
  try {
    const { id } = req.params;
    const [result] = await pool.execute('DELETE FROM books WHERE id = ?', [id]);
    const deleteResult = result as { affectedRows: number };
    
    if (deleteResult.affectedRows === 0) {
      return res.status(404).json({ error: 'Book not found' });
    }
    
    res.json({ message: 'Book deleted successfully' });
  } catch (error) {
    console.error('Error deleting book:', error);
    res.status(500).json({ error: 'Failed to delete book' });
  }
});

export default router;
