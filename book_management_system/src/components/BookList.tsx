import type { Book } from '../types/book';

interface BookListProps {
  books: Book[];
  onEdit: (book: Book) => void;
  onDelete: (id: number) => void;
}

export function BookList({ books, onEdit, onDelete }: BookListProps) {
  if (books.length === 0) {
    return (
      <div className="text-center py-12">
        <p className="text-gray-500 text-lg">暂无图书数据</p>
      </div>
    );
  }

  return (
    <div className="overflow-x-auto">
      <table className="w-full text-sm text-left">
        <thead className="text-xs text-gray-700 uppercase bg-gray-50">
          <tr>
            <th className="px-4 py-3">书名</th>
            <th className="px-4 py-3">作者</th>
            <th className="px-4 py-3">ISBN</th>
            <th className="px-4 py-3">分类</th>
            <th className="px-4 py-3">价格</th>
            <th className="px-4 py-3">出版日期</th>
            <th className="px-4 py-3">操作</th>
          </tr>
        </thead>
        <tbody>
          {books.map((book) => (
            <tr key={book.id} className="border-b hover:bg-gray-50">
              <td className="px-4 py-3 font-medium text-blue-600">{book.title}</td>
              <td className="px-4 py-3">{book.author}</td>
              <td className="px-4 py-3 text-gray-500">{book.isbn}</td>
              <td className="px-4 py-3">
                <span className="px-2 py-1 text-xs font-medium bg-green-100 text-green-800 rounded">
                  {book.category}
                </span>
              </td>
              <td className="px-4 py-3 font-medium">¥{Number(book.price).toFixed(2)}</td>
              <td className="px-4 py-3">{book.published_date}</td>
              <td className="px-4 py-3">
                <div className="flex space-x-2">
                  <button
                    onClick={() => onEdit(book)}
                    className="px-3 py-1 text-xs font-medium text-white bg-blue-500 rounded hover:bg-blue-600 transition-colors"
                  >
                    编辑
                  </button>
                  <button
                    onClick={() => onDelete(book.id)}
                    className="px-3 py-1 text-xs font-medium text-white bg-red-500 rounded hover:bg-red-600 transition-colors"
                  >
                    删除
                  </button>
                </div>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
