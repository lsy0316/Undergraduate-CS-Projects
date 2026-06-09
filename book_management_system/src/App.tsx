import { useState, useMemo, useEffect } from 'react';
import type { Book } from './types/book';
import { fetchBooks, createBook, updateBook, deleteBook } from './api/books';
import { BookList } from './components/BookList';
import { BookForm } from './components/BookForm';
import { SearchBar } from './components/SearchBar';

function App() {
  const [books, setBooks] = useState<Book[]>([]);
  const [searchTerm, setSearchTerm] = useState('');
  const [isFormOpen, setIsFormOpen] = useState(false);
  const [editingBook, setEditingBook] = useState<Book | null>(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadBooks();
  }, []);

  const loadBooks = async (search?: string) => {
    setLoading(true);
    try {
      const data = await fetchBooks(search);
      setBooks(data);
    } catch (error) {
      console.error('Error loading books:', error);
    } finally {
      setLoading(false);
    }
  };

  const filteredBooks = useMemo(() => {
    if (!searchTerm.trim()) {
      return books;
    }
    const term = searchTerm.toLowerCase();
    return books.filter(
      book =>
        book.title.toLowerCase().includes(term) ||
        book.author.toLowerCase().includes(term)
    );
  }, [books, searchTerm]);

  const handleSearchChange = (term: string) => {
    setSearchTerm(term);
  };

  const handleAddBook = () => {
    setEditingBook(null);
    setIsFormOpen(true);
  };

  const handleEditBook = (book: Book) => {
    setEditingBook(book);
    setIsFormOpen(true);
  };

  const handleDeleteBook = async (id: number) => {
    if (window.confirm('确定要删除这本书吗？')) {
      try {
        await deleteBook(id.toString());
        setBooks(prev => prev.filter(book => book.id !== id));
      } catch (error) {
        console.error('Error deleting book:', error);
        alert('删除失败');
      }
    }
  };

  const handleFormSubmit = async (formData: {
    title: string;
    author: string;
    isbn: string;
    price: number;
    category: string;
    description: string;
    publishedDate: string;
    id?: number;
  }) => {
    try {
      if (formData.id) {
        await updateBook(formData.id.toString(), {
          title: formData.title,
          author: formData.author,
          isbn: formData.isbn,
          price: formData.price,
          category: formData.category,
          description: formData.description,
          published_date: formData.publishedDate,
        });
        setBooks(prev =>
          prev.map(book =>
            book.id === formData.id
              ? { ...book, ...formData, published_date: formData.publishedDate }
              : book
          )
        );
      } else {
        const newBook = await createBook({
          title: formData.title,
          author: formData.author,
          isbn: formData.isbn,
          price: formData.price,
          category: formData.category,
          description: formData.description,
          published_date: formData.publishedDate,
        });
        setBooks(prev => [...prev, newBook]);
      }
      setIsFormOpen(false);
      setEditingBook(null);
    } catch (error) {
      console.error('Error saving book:', error);
      alert('保存失败');
    }
  };

  const handleFormCancel = () => {
    setIsFormOpen(false);
    setEditingBook(null);
  };

  return (
    <div className="min-h-screen bg-gray-100">
      <header className="bg-blue-600 text-white py-6">
        <div className="max-w-6xl mx-auto px-4">
          <h1 className="text-3xl font-bold">图书管理系统</h1>
          <p className="text-blue-100 mt-1">管理您的图书收藏</p>
        </div>
      </header>

      <main className="max-w-6xl mx-auto px-4 py-8">
        <SearchBar
          searchTerm={searchTerm}
          onSearchChange={handleSearchChange}
          onAddClick={handleAddBook}
        />

        {loading ? (
          <div className="text-center py-12">
            <div className="inline-block animate-spin rounded-full h-8 w-8 border-4 border-blue-600 border-t-transparent"></div>
            <p className="mt-4 text-gray-500">加载中...</p>
          </div>
        ) : (
          <div className="bg-white rounded-lg shadow-md overflow-hidden">
            <BookList
              books={filteredBooks}
              onEdit={handleEditBook}
              onDelete={handleDeleteBook}
            />
          </div>
        )}

        <div className="mt-4 text-sm text-gray-500">
          共 {filteredBooks.length} 本图书
        </div>
      </main>

      {isFormOpen && (
        <BookForm
          book={editingBook}
          onSubmit={handleFormSubmit}
          onCancel={handleFormCancel}
        />
      )}
    </div>
  );
}

export default App;
