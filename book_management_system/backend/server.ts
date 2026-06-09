import express, { type Request, type Response } from 'express';
import cors from 'cors';
import dotenv from 'dotenv';
import booksRouter from './routes/books';

dotenv.config();

const app = express();
const PORT = process.env.SERVER_PORT || 3001;

app.use(cors());
app.use(express.json());

app.use('/api/books', booksRouter);

app.get('/', (_req: Request, res: Response) => {
  res.json({ message: 'Book Management System API' });
});

app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
