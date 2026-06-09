import type { Book } from '../types/book';

const API_BASE_URL = 'http://localhost:3001/api';

export async function fetchBooks(search?: string): Promise<Book[]> {
  const url = new URL(`${API_BASE_URL}/books`);
  if (search) {
    url.searchParams.set('search', search);
  }
  const response = await fetch(url.toString());
  if (!response.ok) {
    throw new Error('Failed to fetch books');
  }
  return response.json();
}

export async function fetchBook(id: string): Promise<Book> {
  const response = await fetch(`${API_BASE_URL}/books/${id}`);
  if (!response.ok) {
    throw new Error('Failed to fetch book');
  }
  return response.json();
}

export async function createBook(book: Omit<Book, 'id'>): Promise<Book> {
  const response = await fetch(`${API_BASE_URL}/books`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(book),
  });
  if (!response.ok) {
    throw new Error('Failed to create book');
  }
  return response.json();
}

export async function updateBook(id: string, book: Omit<Book, 'id'>): Promise<Book> {
  const response = await fetch(`${API_BASE_URL}/books/${id}`, {
    method: 'PUT',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(book),
  });
  if (!response.ok) {
    throw new Error('Failed to update book');
  }
  return response.json();
}

export async function deleteBook(id: string): Promise<void> {
  const response = await fetch(`${API_BASE_URL}/books/${id}`, {
    method: 'DELETE',
  });
  if (!response.ok) {
    throw new Error('Failed to delete book');
  }
}
