macro_rules! impl_iterator {
    ($name:ident, $self_trait:ident, $item_trait:ident, $unchecked:ident, $item_ty:ident, $assoc:ident) => {
        pub struct $name<'a, T, Item: 'a + $item_trait<T = T>, G: $self_trait<T = T, $assoc<'a> = Item>> {
            geom: &'a G,
            index: usize,
            end: usize,
        }

        impl<'a, T, Item: 'a + $item_trait<T = T>, G: $self_trait<T = T, $assoc<'a> = Item>> $name<'a, T, Item, G> {
            pub fn new(geom: &'a G, index: usize, end: usize) -> Self {
                Self { geom, index, end }
            }
        }

        impl<'a, T, Item: 'a + $item_trait<T = T>, G: $self_trait<T = T, $assoc<'a> = Item>> Iterator
            for $name<'a, T, Item, G>
        {
            type Item = Item;

            fn next(&mut self) -> Option<Self::Item> {
                if self.index == self.end { return None; }
                let i = self.index;
                self.index += 1;
                unsafe { Some(self.geom.$unchecked(i)) }
            }

            fn size_hint(&self) -> (usize, Option<usize>) {
                let n = self.end - self.index;
                (n, Some(n))
            }
        }

        impl<'a, T, Item: 'a + $item_trait<T = T>, G: $self_trait<T = T, $assoc<'a> = Item>> ExactSizeIterator
            for $name<'a, T, Item, G> {}

        impl<'a, T, Item: 'a + $item_trait<T = T>, G: $self_trait<T = T, $assoc<'a> = Item>> DoubleEndedIterator
            for $name<'a, T, Item, G>
        {
            fn next_back(&mut self) -> Option<Self::Item> {
                if self.index == self.end { return None; }
                self.end -= 1;
                unsafe { Some(self.geom.$unchecked(self.end)) }
            }
        }
    };
}

pub(crate) use impl_iterator;
