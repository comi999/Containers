// Container Functions
// - Append
// - AsEnumerable
// - AsCEnumerable
// - AsRefArray
// - AsCRefArray
// - AsReadOnly
// - Assign
// - At
// - Back
// - Capacity
// - Clear
// - Combine
// - CopyTo
// - Data
// - Difference
// - Emplace
// - EmplaceAt
// - EmplaceBack
// - EmplaceFront
// - EmplaceZeroed
// - EmplaceZeroedAt
// - Empty
// - Equals
// - Exists
// - Find
// - FindAll
// - FindLast
// - ForEach
// - Front
// - IndexOf
// - Insert
// - InsertAt
// - Intersection
// - IsValidIndex
// - MaxSize
// - MemCopy
// - MemSet
// - Partition
// - PopBack
// - PushBack
// - PushFront
// - QuickRemove
// - QuickRemoveAll
// - Remove
// - RemoveAll
// - Reserve
// - Resize
// - Reverse
// - Shrink
// - ShrinkToFit
// - Size
// - Sort
// - SubArray
// - Swap
// - Symmetry
// - ToArray
// - ToList
// - ToDictionary
// - ToSet
// - ToPartitionArray
// - TrueForAll
// - Underlying
// - Union
// - Zero
// - ZeroAll
// - operator[] const V& (size_t) const

// Container Types:
// - Array
// - ReadOnlyArray
// - Enumerable
// - RefArray
// - CRefArray
// - Set
// - List
// - Dictionary
// - PartitionArray

// ReadOnlyArray
// - ReadOnlyArray (V[])
// - ReadOnlyArray (InitializerList<V>&)
// - ReadOnlyArray (InitializerList<V>&&)
// - ReadOnlyArray (Container&)
// - ReadOnlyArray (Iter, const Iter&)
// - AsCEnumerable CEnumerable () const
// - AsCRefArray CRefArray () const
// - At const V& (size_t) const
// - Back const V& (size_t) const
// - Capacity size_t () const
// - Combine CRefArray<V> (const ReadOnlyArray&) const
// - CopyTo void (Container&) const
// - Data const V* () const
// - Difference CRefArray<V> (Container&) const
// - Empty bool () const
// - Equals bool (Container&) const
// - Exists bool (const Predicate<const V&>&) const
// - Find CIterator (const V&) const
// - Find CIterator (const Predicate<const V&>&) const
// - Find CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindAll Array<CIterator> (const Predicate<const V&>&) const
// - FindAll Array<CIterator> (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindLast CIterator (const V&) const
// - FindLast CIterator (const Predicate<const V&>&) const
// - FindLast CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - ForEach void (const Action<const V&>&) const
// - Front const V& () const
// - IndexOf int (const V&) const
// - IndexOf int (const CIterator&) const
// - IndexOf int (const Predicate<const V&>&) const
// - Intersection CRefArray<V> (Container&) const
// - IsValidIndex bool (size_t) const
// - MaxSize size_t () const
// - Partition CRefArray<V> (size_t, size_t) const
// - Reverse void ()
// - Reverse void (const CIterator&, const CIterator&)
// - Size size_t () const
// - Sort void ()
// - Sort void (CIterator&, CIterator&)
// - SubArray ReadOnlyArray& <size_t,size_t>()
// - SubArray const ReadOnlyArray& <size_t,size_t>() const
// - Swap void (CIterator&, CIterator&)
// - Swap void (ReadOnlyArray&)
// - Symmetry CRefArray<V> (Container&) const
// - TrueForAll bool (const Predicate<const V&>&) const
// - Underlying const array<V>& () const
// - Union CRefArray<V> (Container&) const
// - operator[] const V& (size_t) const

// Array(array)
// - Array (V[])
// - Array (InitializerList<V>&)
// - Array (InitializerList<V>&&)
// - Array (Container&)
// - Array (Iter, const Iter&)
// - AsEnumerable Enumerable<V> ()
// - AsEnumerable CEnumerable<V> () const
// - AsCEnumerable CEnumerable<V> () const
// - AsRefArray RefArray<V> ()
// - AsRefArray CRefArray<V> () const
// - AsCRefArray CRefArray<V> () const
// - AsReadOnly ReadOnlyArray<V> ()
// - AsReadOnly const ReadOnlyArray<V> () const
// - Assign void (V&)
// - Assign void (const V&)
// - Assign void (Iterator, const Iterator&, V&)
// - Assign void (Iterator, const Iterator&, const V&)
// - At V& (size_t)
// - At const V& (size_t) const
// - Back V& (size_t)
// - Back const V& (size_t) const
// - Capacity size_t () const
// - Combine RefArray<V> (Container&)
// - Combine CRefArray<V> (const Container&) const
// - CopyTo void (Container&)
// - CopyTo void (Container&) const
// - Data V* ()
// - Data const V* () const
// - Difference RefArray<V> (Container&)
// - Difference CRefArray<V> (Container&) const
// - EmplaceAt void <T...>(Iterator&, T&&...) 
// - EmplaceZeroedAt void (Iterator&)
// - Equals bool (Container&) const
// - Exists bool (const V&) const
// - Exists bool (const Predicate<const V&>&) const
// - Find Iterator (const V&)
// - Find CIterator (const V&) const
// - Find Iterator (const Predicate<const V&>&)
// - Find CIterator (const Predicate<const V&>&) const
// - Find Iterator (Iterator, const Iterator&, const Predicate<const V&>&)
// - Find CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindAll Array<Iterator> (const Predicate<const V&>&)
// - FindAll Array<CIterator> (const Predicate<const V&>&) const
// - FindAll Array<Iterator> (Iterator, const Iterator&, const Predicate<const V&>&)
// - FindAll Array<CIterator> (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindLast Iterator (const V&)
// - FindLast CIterator (const V&) const
// - FindLast Iterator (const Predicate<const V&>&)
// - FindLast CIterator (const Predicate<const V&>&) const
// - FindLast Iterator (Iterator, const Iterator&, const Predicate<const V&>&)
// - FindLast CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - ForEach void (const Action<V&>&)
// - ForEach void (const Action<const V&>&) const
// - Front V& ()
// - Front const V& () const
// - IndexOf size_t (const V&) const
// - IndexOf size_t (const CIterator&) const
// - IndexOf size_t (const Predicate<const V&>&) const
// - InsertAt void (CIterator&, V&)
// - InsertAt void (CIterator&, const V&)
// - Intersection RefArray<V> (Container&)
// - Intersection CRefArray<V> (Container&) const
// - IsValidIndex bool (size_t) const
// - MaxSize size_t () const
// - MemCopy void (const V*, size_t, size_t)
// - MemSet void (char, size_t, size_t)
// - Partition RefArray<V> (size_t, size_t)
// - Partition CRefArray<V> (size_t, size_t) const
// - Reverse void ()
// - Reverse void (const CIterator&, const CIterator&)
// - Size size_t () const
// - Sort void <bool>()
// - Sort void <bool>(CIterator&, CIterator&)
// - SubArray Array<V>& <size_t, size_t>()
// - SubArray const Array<V>& <size_t, size_t>() const
// - Swap void (Array<V>&)
// - Swap void (CIterator&, CIterator&)
// - Symmetry RefArray<V> (Container&)
// - Symmetry CRefArray<V> (Container&) const
// - ToArray Array<V> () const
// - ToList List<V> () const
// - ToDictionary Dictionary<U, V> (const Invoker<U, const V&>&) const
// - ToDictionary Dictionary<U, W> (const Invoker<U, const V&>&, const Invoker<W, const V&>&) const
// - ToSet Set<V> () const
// - ToPartitionArray PartitionArray<V> () const
// - TrueForAll bool (const Predicate<V&>&)
// - TrueForAll bool (const Predicate<const V&>&) const
// - Underlying array<V,size_t> ()
// - Underlying const array<V,size_t> () const
// - Union RefArray<V> (Container&)
// - Union CRefArray<V> (Container&) const
// - Zero void (Iterator&)
// - Zero void (Iterator&, Iterator&)
// - ZeroAll void ()
// - operator[] V& (size_t)
// - operator[] const V& (size_t) const

// Array(vector)
// - Array (V[])
// - Array (V&, size_t)
// - Array (const V&, size_t)
// - Array (InitializerList<V>&)
// - Array (InitializerList<V>&&)
// - Array (Container&)
// - Array (Iter, const Iter&)
// - Append (V[])
// - Append (V&, size_t)
// - Append (const V&, size_t)
// - Append (Container&)
// - AsEnumerable Enumerable<V> ()
// - AsEnumerable CEnumerable<V> () const
// - AsCEnumerable CEnumerable<V> () const
// - AsRefArray RefArray<V> ()
// - AsRefArray CRefArray<V> () const
// - AsCRefArray CRefArray<V> () const
// - Assign void (V&)
// - Assign void (const V&)
// - Assign void (Iterator, const Iterator&, V&)
// - Assign void (Iterator, const Iterator&, const V&)
// - At V& (size_t)
// - At const V& (size_t) const
// - Back V& (size_t)
// - Back const V& (size_t) const
// - Capacity size_t () const
// - Clear void ()
// - Combine RefArray<V> (Container&)
// - Combine CRefArray<V> (Container&) const
// - CopyTo void (Container&)
// - CopyTo void (Container&) const
// - Data V* ()
// - Data const V* () const
// - Difference RefArray<V> (Container&)
// - Difference CRefArray<V> (Container&) const
// - Emplace void <T...>(Iterator&, T&&...)
// - EmplaceAt void <T...>(Iterator&, T&&...)
// - EmplaceBack void <T...>(T&&...)
// - EmplaceFront void <T...>(T&&...)
// - EmplaceZeroed void (Iterator&)
// - EmplaceZeroedAt void (Iterator&)
// - Empty bool () const
// - Equals bool (Container&) const
// - Exists bool (const V&) const
// - Exists bool (const Predicate<const V&>&) const
// - Find Iterator (const V&)
// - Find CIterator (const V&) const
// - Find Iterator (const Predicate<const V&>&)
// - Find CIterator (const Predicate<const V&>&) const
// - Find Iterator (Iterator, const Iterator&, const Predicate<const V&>&)
// - Find CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindAll Array<Iterator> (const Predicate<const V&>&)
// - FindAll Array<CIterator> (const Predicate<const V&>&) const
// - FindAll Array<Iterator> (Iterator, const Iterator&, const Predicate<const V&>&)
// - FindAll Array<CIterator> (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindLast Iterator (const V&)
// - FindLast CIterator (const V&) const
// - FindLast Iterator (const Predicate<const V&>&)
// - FindLast CIterator (const Predicate<const V&>&) const
// - FindLast Iterator (Iterator, const Iterator&, const Predicate<const V&>&)
// - FindLast CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - ForEach void (const Action<V&>&)
// - ForEach void (const Action<const V&>&) const
// - Front V& ()
// - Front const V& () const
// - IndexOf size_t (const V&) const
// - IndexOf size_t (const CIterator&) const
// - IndexOf size_t (const Predicate<const V&>&) const
// - Insert void (const CIterator&, V&)
// - Insert void (const CIterator&, const V&)
// - InsertAt void (Iterator&, V&)
// - InsertAt void (Iterator&, const V&)
// - Intersection RefArray<V> (Container&)
// - Intersection CRefArray<V> (Container&) const
// - IsValidIndex bool (size_t) const
// - MaxSize size_t () const
// - MemCopy void (const V*, size_t, size_t)
// - MemSet void (char, size_t, size_t)
// - Partition RefArray<V> (size_t, size_t)
// - Partition CRefArray<V> (size_t, size_t) const
// - PopBack void ()
// - PushBack void (V&)
// - PushBack void (const V&)
// - PushFront void (V&)
// - PushFront void (const V&)
// - QuickRemove void (const CIterator&)
// - QuickRemoveAll void (const Predicate<const V&>&)
// - Remove void (const CIterator&)
// - RemoveAll void (const Predicate<const V&>&)
// - Reserve void (size_t)
// - Resize void (size_t)
// - Reverse void ()
// - Reverse void (const CIterator&, const CIterator&)
// - Shrink void (const CIterator&, const CIterator&)
// - ShrinkToFit void ()
// - Size size_t () const
// - Sort void <bool>()
// - Sort void <bool>(CIterator&, CIterator&)
// - SubArray Array<V>& <size_t, size_t>()
// - SubArray const Array<V>& <size_t, size_t>() const
// - Swap void (Array<V>&)
// - Swap void (CIterator&, CIterator&)
// - Symmetry RefArray<V> (Container&)
// - Symmetry CRefArray<V> (Container&) const
// - ToList List<V> () const
// - ToDictionary Dictionary<U, V> (const Invoker<U, const V&>&) const
// - ToDictionary Dictionary<U, W> (const Invoker<U, const V&>&, const Invoker<W, const V&>&) const
// - ToSet Set<V> () const
// - ToPartitionArray PartitionArray<V> () const
// - TrueForAll bool (const Predicate<V&>&)
// - TrueForAll bool (const Predicate<const V&>&) const
// - Underlying vector<V> ()
// - Underlying const vector<V> () const
// - Union RefArray<V> (Container&)
// - Union CRefArray<V> (Container&) const
// - Zero void (Iterator&)
// - Zero void (Iterator&, Iterator&)
// - ZeroAll void ()
// - operator[] V& (size_t)
// - operator[] const V& (size_t) const

// RefArray
// - RefArray (V[])
// - RefArray (InializerList<V>&)
// - RefArray (InitializerList<V>&&)
// - RefArray (Container&)
// - RefArray (Iter, const Iter&)
// - Append void (V[])
// - Append void (Container&)
// - AsEnumerable Enumerable<V> ()
// - AsEnumerable CEnumerable<V> () const
// - AsCEnumerable CEnumerable<V> () const
// - AsCRefArray CRefArray<V>& () const
// - Assign void (V&)
// - Assign void (const V&)
// - Assign void (Iterator, const Iterator&, V&)
// - Assign void (Iterator, const Iterator&, const V&)
// - At V& (size_t)
// - At const V& (size_t) const
// - Back V& (size_t)
// - Back const V& (size_t) const
// - Capacity size_t () const
// - Clear void ()
// - Combine RefArray<V> (Container&)
// - Combine CRefArray<V> (Container&) const
// - CopyTo void (Container&)
// - CopyTo void (Container&) const
// - Data V* ()
// - Data const V* () const
// - Difference RefArray<V> (Container&)
// - Difference CRefArray<V> (Container&) const
// - EmplaceAt void <T...>(Iterator&, T&&...)
// - EmplaceZeroedAt void (Iterator&)
// - Empty bool () const
// - Equals bool (Container&) const
// - Exists bool (const V&) const
// - Exists bool (const Predicate<const V&>&) const
// - Find Iterator (const V&)
// - Find CIterator (const V&) const
// - Find Iterator (const Predicate<const V&>&)
// - Find CIterator (const Predicate<const V&>&) const
// - Find Iterator (Iterator, const Iterator&, const Predicate<const V&>&)
// - Find CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindAll Array<Iterator> (const Predicate<const V&>&)
// - FindAll Array<CIterator> (const Predicate<const V&>&) const
// - FindAll Array<Iterator> (Iterator, const Iterator&, const Predicate<const V&>&)
// - FindAll Array<CIterator> (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindLast Iterator (const V&)
// - FindLast CIterator (const V&) const
// - FindLast Iterator (const Predicate<const V&>&)
// - FindLast CIterator (const Predicate<const V&>&) const
// - FindLast Iterator (Iterator, const Iterator&, const Predicate<const V&>&)
// - FindLast CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - ForEach void (const Action<V&>&)
// - ForEach void (const Action<const V&>&) const
// - Front V& ()
// - Front const V& () const
// - IndexOf size_t (const V&) const
// - IndexOf size_t (const CIterator&) const
// - IndexOf size_t (const Predicate<const V&>&) const
// - Insert void (const CIterator&, V&)
// - Insert void (const CIterator&, const V&)
// - InsertAt void (Iterator&, V&)
// - InsertAt void (Iterator&, const V&)
// - Intersection RefArray<V> (Container&)
// - Intersection CRefArray<V> (Container&) const
// - IsValidIndex bool (size_t) const
// - MaxSize size_t () const
// - MemCopy void (const V*, size_t, size_t)
// - MemSet void (char, size_t, size_t)
// - Partition RefArray<V> (size_t, size_t)
// - Partition CRefArray<V> (size_t, size_t) const
// - PopBack void ()
// - PushBack void (V&)
// - PushBack void (const V&)
// - PushFront void (V&)
// - PushFront void (const V&)
// - QuickRemove void (const CIterator&)
// - QuickRemoveAll void (const Predicate<const V&>&)
// - Remove void (const CIterator&)
// - RemoveAll void (const Predicate< const V&>&)
// - Reserve void (size_t)
// - Resize void (size_t)
// - Reverse void ()
// - Reverse void (const CIterator&, const CIterator&)
// - Shrink void (const CIterator&, const CIterator&)
// - ShrinkToFit void ()
// - Size size_t () const
// - Sort void <bool>()
// - Sort void <bool>(CIterator&, CIterator&)
// - Swap void (RefArray<V>&)
// - Swap void (CIterator&, CIterator&)
// - Symmetry RefArray<V> (Container&)
// - Symmetry CRefArray<V> (Container&) const
// - ToList List<V> () const
// - ToDictionary Dictionary<U, V> (const Invoker<U, const V&>&) const
// - ToDictionary Dictionary<U, W> (const Invoker<U, const V&>&, const Invoker<W, const V&>&) const
// - ToSet Set<V> () const
// - ToPartitionArray PartitionArray<V> () const
// - TrueForAll bool (const Predicate<V&>&)
// - TrueForAll bool (const Predicate<const V&>&) const
// - Underlying vector<V> ()
// - Underlying const vector<V> () const
// - Union RefArray<V> (Container&)
// - Union CRefArray<V> (Container&) const
// - Zero void (Iterator&)
// - Zero void (Iterator&, Iterator&)
// - ZeroAll void ()
// - operator[] V& (size_t)
// - operator[] const V& (size_t) const

// CRefArray
// - CRefArray (V[])
// - CRefArray (InializerList<V>&)
// - CRefArray (InitializerList<V>&&)
// - CRefArray (Container&)
// - CRefArray (CIter, const CIter&)
// - Append void (V[])
// - Append void (const Container&)
// - AsEnumerable CEnumerable<V> () const
// - AsCEnumerable CEnumerable<V> () const
// - Assign void (V&)
// - Assign void (CIterator, const CIterator&, const V&) const
// - At const V& (size_t) const
// - Back const V& (size_t) const
// - Capacity size_t () const
// - Clear void ()
// - Combine CRefArray<V> (const CRefArray<V>&) const
// - CopyTo void (Container&) const
// - Data const V* () const
// - Difference CRefArray<V> (Container&) const
// - Empty bool () const
// - Equals bool (Container&) const
// - Exists bool (const V&) const
// - Exists bool (const Predicate<const V&>&) const
// - Find CIterator (const V&) const
// - Find CIterator (const Predicate<const V&>&) const
// - Find CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindAll Array<CIterator> (const Predicate<const V&>&) const
// - FindAll Array<CIterator> (CIterator, const CIterator&, const Predicate<const V&>&) const
// - FindLast CIterator (const V&) const
// - FindLast CIterator (const Predicate<const V&>&) const
// - FindLast CIterator (CIterator, const CIterator&, const Predicate<const V&>&) const
// - ForEach void (const Action<const V&>&) const
// - Front const V& () const
// - IndexOf size_t (const V&) const
// - IndexOf size_t (const CIterator&) const
// - IndexOf size_t (const Predicate<const V&>&) const
// - Insert void (CIterator&, V&)
// - Insert void (CIterator&, const V&)
// - Intersection CRefArray<V> (Container&) const
// - IsValidIndex bool (size_t) const
// - MaxSize size_t () const
// - Partition CRefArray<V> (size_t, size_t) const
// - PopBack void ()
// - PushBack void (const V&)
// - PushFront void (const V&)
// - QuickRemove void (CIterator&)
// - QuickRemoveAll void (const Predicate<const V&>&)
// - Remove void (Iterator)
// - RemoveAll void (const Predicate< const V&>&)
// - Reserve void (size_t)
// - Resize void (size_t)
// - Reverse void ()
// - Reverse void (Iterator, Iterator)
// - Shrink void (Iterator, Iterator)
// - ShrinkToFit void ()
// - Size size_t () const
// - Sort void <bool>()
// - Sort void <bool>(Iterator, Iterator)
// - Swap void (RefArray<V>&)
// - Swap void (Iterator, Iterator)
// - Symmetry RefArray<V> (RefArray<V>&)
// - Symmetry CRefArray<V> (const RefArray<V>&) const
// - ToList List<V> () const
// - ToDictionary Dictionary<U, V> (const Invoker<U, const V&>&) const
// - ToDictionary Dictionary<U, W> (const Invoker<U, const V&>&, const Invoker<W, const V&>&) const
// - ToSet Set<V> () const
// - ToPartitionArray PartitionArray<V> () const
// - TrueForAll bool (const Predicate<V&>&)
// - TrueForAll bool (const Predicate<const V&>&) const
// - Underlying vector<V> ()
// - Underlying const vector<V> () const
// - Union RefArray<V> (RefArray<V>&)
// - Union CRefArray<V> (const RefArray<V>&) const
// - Zero void (Iterator&)
// - Zero void (Iterator&, Iterator&)
// - ZeroAll void ()
// - operator[] V& (size_t)
// - operator[] const V& (size_t) const

// Enumerable
// - Enumerable(V[])
// - Enumerable(const Iter&, const Iter&)
// - Enumerable(const Iter&, const Iter&, size_t)
// - Enumerable(Container&)
// - AsCEnumerable CEnumerable<V> () const
// - AsRefArray RefArray<V> ()
// - AsRefArray CRefArray<V> () const
// - AsCRefArray CRefArray<V> () const
// - Assign void (V&)
// - Assign void (const V&)
// - Assign void (Iterator, const Iterator&, V&)
// - Assign void (Iterator, const Iterator&, const V&)
// - At V& (size_t)
// - At const V& (size_t) const
// - Back V& ()
// - Back const V& () const
// - Capacity size_t () const
// - CopyTo void (Container&)
// - CopyTo void (Container&) const
// - Difference RefArray<V> (Container&)
// - Difference CRefArray<V> (Container&) const
// - EmplaceAt void <T...>(Iterator&, T&&...)
// - EmplaceZeroedAt void (Iterator&)
// - Empty
// - Equals
// - Exists
// - Find
// - FindAll
// - FindLast
// - ForEach
// - Front
// - IndexOf
// - Insert
// - InsertAt
// - Intersection
// - IsValidIndex
// - MaxSize
// - MemCopy
// - MemSet
// - Partition
// - PopBack
// - PushBack
// - PushFront
// - QuickRemove
// - QuickRemoveAll
// - Remove
// - RemoveAll
// - Reserve
// - Resize
// - Reverse
// - Shrink
// - ShrinkToFit
// - Size
// - Sort
// - SubArray
// - Swap
// - Symmetry
// - ToArray
// - ToList
// - ToDictionary
// - ToSet
// - ToPartitionArray
// - TrueForAll
// - Underlying
// - Union
// - Zero
// - ZeroAll
// - operator[] const V& (size_t) const